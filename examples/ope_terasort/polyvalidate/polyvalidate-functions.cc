/*
 * polyvalidate-functions.cc
 *
 *  Created on: 5 Jul 2018
 *      Author: scsjd
 */
#include <jsoncpp/json/json.h>
#include "polyvalidate.h"
#include "ValidateMapTask.h"

void run_map_task(file secrets, file inputFile, file outputFile, file timingsFile){
	auto start = std::chrono::high_resolution_clock::now();
	ValidateMapTask mapTask(secrets,inputFile,outputFile);
	mapTask.run();
	auto finish = std::chrono::high_resolution_clock::now();
	auto taskExecutionTime = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
	Json::Value root;
	root["task_execution_time"] = Json::UInt64(taskExecutionTime);
	root["number_decrypted"] = Json::UInt64(mapTask.numberOfInputs);
	root["total_decryption_time"] = Json::UInt64(mapTask.totalDecryptionTime);
	Json::FastWriter writer;
	std::string json = writer.write(root);
	std::ofstream ofs(timingsFile);
	if (ofs.is_open()){
		ofs << json;
		ofs.close();
	}
}
