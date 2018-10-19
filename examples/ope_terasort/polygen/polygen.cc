#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <jsoncpp/json/json.h>
#include "polygen.h"
#include "PolyACDEncrypter.h"

inline bool file_exists (const char* name) {
  struct stat buffer;
  return (stat(name, &buffer) == 0);
}

void usage() {
    std::cerr << "[ERROR] Bad number of parameters" << std::endl;
    std::cout << "    Usage: polygen <security parameter (bits)> <polynomial coefficient size (bits)> <maximum degree of plaintext polynomials> <degree of secret key polynomial> <number of mappers to use> <number of ciphertexts to generate> <path to save encryption secrets> <prefix for output files> <prefix for task timing file> <job timings file>" << std::endl;
}

void invoke_map_task(int tasknum, int mu, int n, int numOutputsPerMapper, const char* secretsPath, const char* outputPrefix, const char* timingsPrefix){
	std::cout << "Map task " << tasknum << ":" << std::endl;
	std::cout << "\tSize of plaintext coefficients=" << mu << std::endl;
	std::cout << "\tDegree of plaintext polynomials=" << n << std::endl;
	std::cout << "\tNumber of outputs to generate=" << numOutputsPerMapper << std::endl;
	std::cout << "\tSecrets=" << secretsPath << std::endl;
	std::cout << "\tOutput Prefix=" << outputPrefix << std::endl;
	std::cout << "\tTimings Prefix=" << timingsPrefix << std::endl;

	file secretsFile = strdup(secretsPath);
	std::cout << "\tOutput Prefix=" << outputPrefix << std::endl;
	std::stringstream outFilename;
	outFilename << outputPrefix << '_' << tasknum;
	file outFile = strdup(outFilename.str().c_str());
	std::cout << "\tOutput file: " << outFile << std::endl;
	std::stringstream timingsFilename;
	timingsFilename << timingsPrefix << '_' << tasknum;
	file timingsFile = strdup(timingsFilename.str().c_str());
	std::cout << "\tTimings file: " << timingsFile << std::endl;
	run_map_task(mu, n, numOutputsPerMapper, secretsFile, outFile, timingsFile);
}

int main(int argc, char **argv) {
	int lambda, mu, n, d;
	std::size_t numMappers, numOutputs, numOutputsPerMapper;
	const char* outputPrefix;
	const char* timingsPrefix;
	const char* secretsPath;
	const char* timingsPath;
	unsigned long totalExecTime=0;
	unsigned long totalEncryptTime=0;
	unsigned long totalEncrypted=0;
	if (argc < 11){
		usage();
		return -1;
	}
	else{
		lambda = atoi(argv[1]);
		mu = atoi(argv[2]);
		n = atoi(argv[3]);
		d = atoi(argv[4]);
		numMappers = strtoul(argv[5],nullptr,0);
		numOutputs = strtoul(argv[6],nullptr,0);
		secretsPath = argv[7];
		outputPrefix = argv[8];
		timingsPrefix = argv[9];
		timingsPath = argv[10];
	}

	auto start = std::chrono::high_resolution_clock::now();

	numOutputsPerMapper = numOutputs/numMappers;

	std::cout << "Starting polygen with the following configuration:" << std::endl;
	std::cout << "\tSecurity parameter: " << lambda << std::endl;
	std::cout << "\tSize of coefficients: " << mu << std::endl;
	std::cout << "\tMax degree of plaintexts: " << n << std::endl;
	std::cout << "\tDegree of key polynomial: " << d << std::endl;
	std::cout << "\tNumber of mappers: " << numMappers << std::endl;
	std::cout << "\tNumber of ciphertexts: " << numOutputs << std::endl;
	std::cout << "\tSecrets path: " << secretsPath << std::endl;
	std::cout << "\tMap task output file prefix: " << outputPrefix << std::endl;
	std::cout << "\tMap task timings file prefix: " << timingsPrefix << std::endl;
	std::cout << "\tJob timings file: " << timingsPath << std::endl;

	//Generate encryption secrets and write to file
	PolyACDEncrypter poly(lambda,mu,d);
	std::string secrets = poly.writeSecretsToJSON();
	std::ofstream ofs(secretsPath,std::ios::out);
	ofs << secrets;
	ofs.close();

	compss_on();
	//invoke map tasks
	for (std::size_t i = 0; i < numMappers; i++){
		invoke_map_task(i, mu, n, numOutputsPerMapper, secretsPath, outputPrefix, timingsPrefix);
	}
	compss_barrier();

	for (std::size_t i = 0; i < numMappers; i++){
		std::stringstream timingsFilename;
		timingsFilename << timingsPrefix << '_' << i;
		std::ifstream timings_ifs(timingsFilename.str());
		if (timings_ifs.is_open()){
			std::string json, line;
			while(getline(timings_ifs,line)) json += line;
			Json::Value root;   // will contains the root value after parsing.
			Json::Reader reader;
			bool parsingSuccessful = reader.parse(json,root);
			if (parsingSuccessful){
				totalExecTime += root["task_execution_time"].asUInt64();
				totalEncryptTime += root["total_encryption_time"].asUInt64();
				totalEncrypted += root["number_encrypted"].asUInt64();
			}
			timings_ifs.close();
		}
	}
	compss_off();

	auto finish = std::chrono::high_resolution_clock::now();
	auto jobExecutionTime = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

	//Write job statistics to file
	bool fileExists = file_exists(timingsPath);
	std::ofstream timings(timingsPath,std::ios::out | std::ios::app);
	if (!fileExists) timings << "jobExecTime,avgTaskTime,avgEncryptTime" << std::endl;
	double avgTaskTime = (double)totalExecTime/numMappers;
	double avgEncryptTime = (double)totalEncryptTime/totalEncrypted;
	timings << jobExecutionTime << "," << avgTaskTime << "," << avgEncryptTime << std::endl;
	return 0;
}
