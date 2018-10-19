/*
 * Copyright 2018 University of Leeds
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 *
 * This is being developed for the TANGO Project: http://tango-project.eu
 */

#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <sys/stat.h>
#include <jsoncpp/json/json.h>
#include <NTL/ZZ.h>
#include "inner.h"
#include "HE1CiphertextCPU.h"

inline bool file_exists (const char* name) {
  struct stat buffer;
  return (stat(name, &buffer) == 0);
}

void usage() {
    std::cerr << "[ERROR] Bad number of parameters" << std::endl;
    std::cout << "    Usage: inner <num mappers> <num ints per line> <path to parameters file> <prefix for map input files> <prefix for map output files> <prefix for map results path> <path to output file> <path to timings file>" << std::endl;
}

void invoke_map_task(int tasknum, int numPerLine, const char* parametersPath, std::string& inputPath, std::string& resultsPath){
	std::cout << "Map task " << tasknum << ":" << std::endl;
	file parametersFile = strdup(parametersPath);
	std::cout << "\tparameters file: " << parametersPath << std::endl;
	std::string inFile = inputPath + '_' + std::to_string(tasknum);
	file splitFile = strdup(inFile.c_str());
	std::cout << "\tinput file: " << inFile << std::endl;
	std::string resultsFile = resultsPath + '_' + std::to_string(tasknum);
	file resFile = strdup(resultsFile.c_str());
	std::cout << "\tresults file: " << resultsFile << std::endl;
	run_map_task(numPerLine, parametersFile, splitFile, resFile);
}

int main(int argc, char **argv) {
	int numPerLine;
	std::size_t numMappers;
	std::string inputPath, outputPath, resultsPath;
	const char* parametersPath;
	const char* timingsPath;
	if (argc < 8){
		usage();
		return -1;
	}
	else{
		numMappers = atoi(argv[1]);
		numPerLine = atoi(argv[2]);
		parametersPath = argv[3];
		inputPath = argv[4];
		resultsPath = argv[5];
		outputPath = argv[6];
		timingsPath = argv[7];
	}

	auto startJob = std::chrono::high_resolution_clock::now();

	std::cout << "Starting inner with the following configuration:" << std::endl;
	std::cout << "\tNumber of workers to use: " << numMappers << std::endl;
	std::cout << "\tDegree of polynomial to compute: " << numPerLine << std::endl;
	std::cout << "\tParameters file: " << parametersPath << std::endl;
	std::cout << "\tInput file prefix: " << inputPath << std::endl;
	std::cout << "\tMap results file prefix: " << resultsPath << std::endl;
	std::cout << "\tOutput file: " << outputPath << std::endl;
	std::cout << "\tTimings file: " << timingsPath << std::endl;

	compss_on();
	//invoke map tasks
	for (std::size_t i = 0; i < numMappers; i++){
		invoke_map_task(i, numPerLine, parametersPath, inputPath, resultsPath);
	}
	compss_barrier();

	//Read in parameters from file
	std::ifstream ifs(parametersPath);
	if (!ifs.is_open()) throw std::ios_base::failure("Could not open cipher parameters file.");
	std::string parameters;
	getline(ifs,parameters);
	ifs.close();

	//Do prefix sum reduce and collate statistics
	HE1CiphertextCPU::setModulus(parameters);
	NTL::ZZ zero;
	HE1CiphertextCPU result(zero);
	unsigned long totalExecTime=0;
	unsigned long totalSumTime=0;
	unsigned long totalAdditions=0;
	unsigned long totalProductTime=0;
	unsigned long totalMultiplications=0;
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
				totalExecTime += root["task_execution_time"].asUInt64();
				totalAdditions += root["number_additions"].asUInt64();
				totalSumTime += root["total_sum_time"].asUInt64();
				totalMultiplications += root["number_multiplications"].asUInt64();
				totalProductTime += root["total_product_time"].asUInt64();
				std::string resultStr = root["result"].asString();
				HE1CiphertextCPU mapOutput(resultStr);
				auto start = std::chrono::high_resolution_clock::now();
				result += mapOutput;
				auto finish = std::chrono::high_resolution_clock::now();
				totalAdditions++;
				totalSumTime+= std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
			}
		}
	}

	compss_off();

	//write computational result to file
	std::ofstream output(outputPath);
	if (output.is_open()){
		output << result << std::endl;
	}
	output.close();

	auto finishJob = std::chrono::high_resolution_clock::now();
	auto jobExecutionTime = std::chrono::duration_cast<std::chrono::nanoseconds>(finishJob-startJob).count();

	//write out statistics to file
	bool fileExists = file_exists(timingsPath);
	std::ofstream timings(timingsPath,std::ios::out | std::ios::app);
	if (!fileExists) timings << "jobExecTime,avgTaskTime,avgSumTime,avgProductTime" << std::endl;
	double avgTaskTime = (double)totalExecTime/numMappers;
	double avgSumTime = (double)totalSumTime/totalAdditions;
	double avgProductTime = (double)totalProductTime/totalMultiplications;
	timings << jobExecutionTime << "," << std::fixed << avgTaskTime << "," << avgSumTime << "," << avgProductTime << std::endl;
	return 0;
}
