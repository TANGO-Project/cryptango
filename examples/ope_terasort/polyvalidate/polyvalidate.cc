#include "polyvalidate.h"
#include <sys/stat.h>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <jsoncpp/json/json.h>
#include "PolyCiphertext.h"

inline bool file_exists (const char* name) {
  struct stat buffer;
  return (stat(name, &buffer) == 0);
}

void usage() {
    std::cerr << "[ERROR] Bad number of parameters" << std::endl;
    std::cout << "    Usage: polyvalidate <number of workers> <secrets file path> <map input file prefix> <map output file prefix> <map timings file prefix> <output file path> <timings file path>" << std::endl;
}

int main(int argc, char **argv) {
	std::size_t numMappers;
	const char* inputPrefix;
	const char* stagingPrefix;
	const char* outputPath;
	const char* secretsPath;
	const char* timingsPrefix;
	const char* timingsPath;
	if (argc < 8) {
		usage();
		return -1;
	} else {
		numMappers = strtoul(argv[1], nullptr, 0);
		secretsPath = argv[2];
		inputPrefix = argv[3];
		stagingPrefix = argv[4];
		timingsPrefix = argv[5];
		outputPath = argv[6];
		timingsPath = argv[7];
	}

	std::cout << "Starting polyvalidate with the following configuration:" << std::endl;
	std::cout << "\tNumber of mappers: " << numMappers << std::endl;
	std::cout << "\tSecrets path: " << secretsPath << std::endl;
	std::cout << "\tMap task input file prefix: " << inputPrefix << std::endl;
	std::cout << "\tMap task output file prefix: " << stagingPrefix << std::endl;
	std::cout << "\tMap task timings file prefix: " << timingsPrefix << std::endl;
	std::cout << "\tOutput file: " << outputPath << std::endl;
	std::cout << "\tJob statistics file: " << timingsPath << std::endl;

	auto start = std::chrono::high_resolution_clock::now();

	compss_on();
	file secretsFile = strdup(secretsPath);

	//invoke map tasks
	for (std::size_t i = 0; i < numMappers; i++) {
		std::cout << "Map task " << i << ":" << std::endl;
		std::cout << "\tSecrets=" << secretsPath << std::endl;
		std::stringstream inputFilename;
		inputFilename << inputPrefix << '_' << i;
		file inputFile = strdup(inputFilename.str().c_str());
		std::cout << "\tInput=" << inputFile << std::endl;
		std::stringstream outputFilename;
		outputFilename << stagingPrefix << '_' << i;
		file outputFile = strdup(outputFilename.str().c_str());
		std::cout << "\tOutput=" << outputFile << std::endl;
		std::stringstream timingsFilename;
		timingsFilename << timingsPrefix << '_' << i;
		file timingsFile = strdup(timingsFilename.str().c_str());
		std::cout << "\tTimings=" << timingsFile << std::endl;
		run_map_task(secretsFile, inputFile, outputFile, timingsFile);
	}
	compss_barrier();

	unsigned long totalExecTime=0;
	unsigned long totalDecryptTime=0;
	unsigned long totalDecrypted=0;
	for(std::size_t i = 0; i < numMappers; i++){
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
				totalDecryptTime += root["total_decryption_time"].asUInt64();
				totalDecrypted += root["number_decrypted"].asUInt64();
			}
			timings_ifs.close();
		}
	}

	//collate output from map tasks and check partitioning
	//create output stream
	std::ofstream ofs(outputPath, std::ios::out);
	bool firstKey = true;
	PolyCiphertext lastPoly;
	std::string lastKey, lastValue;
	for (std::size_t i = 0; i < numMappers; i++) {
		//open input stream from file
		std::stringstream outputFilename;
		outputFilename << stagingPrefix << '_' << i;
		std::ifstream ifs(outputFilename.str(), std::ios::in);
		std::string line;
		//read in the file a line at a time
		while (getline(ifs, line)) {
			std::stringstream ss(line);
			std::string key, value;

			//split the line into tab separated fields
			getline(ss, key, '\t');
			getline(ss, value, '\t');

			//pass errors from map phase to output
			if (key.compare("error") == 0) {
				ofs << key << '\t' << value << std::endl;
			} else {
				//check partitioning: map phase outputs a key value pair that denotes the first value in the file
				if (firstKey) {
					firstKey = false;
				} else {
					PolyCiphertext thisPoly(value);
					if (thisPoly < lastPoly) {
						ofs << "error\tbad key partitioning:\n  file " << lastKey << " key " << lastValue << "\n  file " << key << " key " << value << std::endl;
					}
					lastPoly = thisPoly;
				}
			}
			lastKey = key;
			lastValue = value;
		}
	}
	ofs.close();
	compss_off();

	auto finish = std::chrono::high_resolution_clock::now();
	auto jobExecutionTime = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

	bool fileExists = file_exists(timingsPath);
	std::ofstream timings(timingsPath,std::ios::out | std::ios::app);
	if (!fileExists) timings << "jobExecTime,avgTaskTime,avgDecryptTime" << std::endl;
	double avgTaskTime = (double)totalExecTime/numMappers;
	double avgDecryptTime = (double)totalDecryptTime/totalDecrypted;
	timings << jobExecutionTime << "," << avgTaskTime << "," << avgDecryptTime << std::endl;

	return 0;
}
