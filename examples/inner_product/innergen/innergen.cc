#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <sys/stat.h>
#include <jsoncpp/json/json.h>
#include "innergen.h"
#include "HE1NEncrypter.h"

inline bool file_exists (const char* name) {
  struct stat buffer;
  return (stat(name, &buffer) == 0);
}

void usage() {
    std::cerr << "[ERROR] Bad number of parameters" << std::endl;
    std::cout << "    Usage: innergen <number of lines of output per mapper>  <degree of polynomial to compute> <bit length of inputs> <effective bit length of inputs> <number of mappers to use> <path to save cipher secrets> <path to save cipher parameters> <prefix for output files> <result file path> <timings file path>" << std::endl;
}

void invoke_map_task(std::size_t tasknum, int rho, int d, std::size_t numOutputsPerMapper, char* secretsFile, char* parametersFile, std::string& outputPath, std::string& resultsPath){
	std::cout << "Map task " << tasknum << ":" << std::endl;
	std::cout << "\trho=" << rho << std::endl;
	std::cout << "\tnumOutputs=" << numOutputsPerMapper << std::endl;
	std::cout << "\tsecrets file: " << secretsFile << std::endl;
	std::cout << "\tparameters file: " << parametersFile << std::endl;
	std::string outFile = outputPath + '_' + std::to_string(tasknum);
	file splitFile = strdup(outFile.c_str());
	std::cout << "\toutput file: " << splitFile << std::endl;
	std::string resultsFile = resultsPath + '_' + std::to_string(tasknum);
	file resFile = strdup(resultsFile.c_str());
	std::cout << "\tresults file: " << resFile << std::endl;
	run_map_task(rho, d, (int)numOutputsPerMapper, secretsFile, parametersFile, splitFile, resFile);
}

int main(int argc, char **argv) {
	int d, rho, rhoprime;
	std::size_t numMappers, numLinesPerMapper;
	//std::size_t numLeftOver;
	std::string outputPath;
	std::string resultsPath = "results/results";
	const char* secretsPath;
	const char* parametersPath;
	const char* resultPath;
	const char* timingsPath;
	if (argc < 11){
		usage();
		return -1;
	}
	else{
		numLinesPerMapper = strtoul(argv[1],nullptr,0);
		d = atoi(argv[2]);
		rho = atoi(argv[3]);
		rhoprime = atoi(argv[4]);
		numMappers = strtoul(argv[5],nullptr,0);
		secretsPath = argv[6];
		parametersPath = argv[7];
		outputPath = argv[8];
		resultPath = argv[9];
		timingsPath = argv[10];
	}

	auto start = std::chrono::high_resolution_clock::now();

	std::cout << "Starting innergen with the following configuration:" << std::endl;
	std::cout << "\tnumber of lines of output to generate per mapper: " << numLinesPerMapper << std::endl;
	std::cout << "\tdegree of polynomial to compute (integers per line): " << d << std::endl;
	std::cout << "\tbit length of plaintexts: " << rho << std::endl;
	std::cout << "\teffective bit length of plaintexts after encryption: " << rhoprime << std::endl;
	std::cout << "\tnumber of mappers: "<< numMappers << std::endl;
	std::cout << "\tpath to write cipher secrets: " << secretsPath << std::endl;
	std::cout << "\tpath to write cipher parameters: " << parametersPath << std::endl;
	std::cout << "\toutput file prefix: " << outputPath << std::endl;
	std::cout << "\tmap results file prefix: " << resultsPath << std::endl;
	std::cout << "\tpath to write result: " << resultPath << std::endl;
	std::cout << "\tpath ro write timings: " << timingsPath << std::endl;

	//Generate encryption secrets
	std::cout << "Generating keys and parameters" << std::endl;
	std::size_t numOutputs = numLinesPerMapper*numMappers*d;
	HE1NEncrypter he1n(numOutputs,d,rho,rhoprime);

	//Write cipher secrets to file
	std::string secrets = he1n.writeSecretsToJSON();
	std::ofstream secrets_ofs(secretsPath,std::ios::out);
	secrets_ofs << secrets;
	secrets_ofs.close();
	std::cout << "Cipher secrets:" << std::endl;
	std::cout << secrets;

	//Write cipher parameters to file
	std::string parameters = he1n.writeParametersToJSON();
	std::ofstream params_ofs(parametersPath,std::ios::out);
	params_ofs << parameters;
	params_ofs.close();
	std::cout << "Cipher parameters:" << std::endl;
	std::cout << parameters;

	compss_on();
	//invoke map tasks
	file secretsFile = strdup(secretsPath);
	file paramsFile = strdup(parametersPath);
	//invoke_map_task(0, rho, d, numOutputsPerMapper+numLeftOver, secretsFile, paramsFile, outputPath, resultsPath);
	for (std::size_t i = 0; i < numMappers; i++){
		invoke_map_task(i, rho, d, numLinesPerMapper, secretsFile, paramsFile, outputPath, resultsPath);
	}

	compss_barrier();
	//Retrieve computational results and statistics from tasks
	NTL::ZZ result;
	unsigned long totalExecTime=0;
	unsigned long totalEncryptTime=0;
	unsigned long totalEncrypted=0;
	for (std::size_t i = 0; i < numMappers; i++){
		std::string resultsFile = resultsPath + '_' + std::to_string(i);
		std::ifstream results_ifs(resultsFile);
		if (results_ifs.is_open()){
			std::string json, line;
			while(getline(results_ifs,line)) json += line;
			Json::Value root;   // will contains the root value after parsing.
			Json::Reader reader;
			bool parsingSuccessful = reader.parse(json,root);
			if (parsingSuccessful){
				NTL::ZZ mapResult = NTL::conv<NTL::ZZ>(root["result"].asCString());
				result += mapResult;
				totalExecTime += root["task_execution_time"].asLargestUInt();
				totalEncryptTime += root["total_encryption_time"].asLargestUInt();
				totalEncrypted += root["number_of_inputs"].asLargestUInt();
			}
		}
	}
	compss_off();

	//Write computational result to file
	std::ofstream resultFile(resultPath);
	if (resultFile.is_open()){
		resultFile << result << std::endl;
	}
	resultFile.close();

	auto finish = std::chrono::high_resolution_clock::now();
	auto jobExecutionTime = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

	bool fileExists = file_exists(timingsPath);
	std::ofstream timings(timingsPath,std::ios::out | std::ios::app);
	if (!fileExists) timings << "jobExecTime,avgTaskTime,avgEncryptTime" << std::endl;
	double avgTaskTime = (double)totalExecTime/numMappers;
	double avgEncryptTime = (double)totalEncryptTime/totalEncrypted;
	timings << jobExecutionTime << "," << std::fixed << avgTaskTime << "," << avgEncryptTime << std::endl;
	return 0;
}
