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

#include <chrono>
#include <fstream>
#include <sstream>
#include <jsoncpp/json/json.h>
#include <NTL/ZZ.h>
#include "inner.h"
#include "InnerMapTask.h"

void run_map_task(int numPerLine, file parametersPath, file inputPath, file resultsPath){
	auto start = std::chrono::high_resolution_clock::now();
	InnerMapTask mapTask(numPerLine, parametersPath, inputPath);
	NTL::ZZ result = mapTask.run();
	auto finish = std::chrono::high_resolution_clock::now();
	auto totalTaskTime = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
	Json::Value root;
	std::stringstream ss;
	ss << result;
	root["result"] = ss.str();
	root["task_execution_time"] = Json::UInt64(totalTaskTime);
	root["number_additions"] = Json::UInt64(mapTask.numberAdditions);
	root["total_sum_time"] = Json::UInt64(mapTask.totalSumTime);
	root["number_multiplications"] = Json::UInt64(mapTask.numberMultiplications);
	root["total_product_time"] = Json::UInt64(mapTask.totalProductTime);
	std::ofstream results_ofs(resultsPath);
	if(results_ofs.is_open()){
		results_ofs << root;
		results_ofs.close();
	}
}

void run_map_task_GPU(int numPerLine, file parametersPath, file inputPath, file resultsPath){
	auto start = std::chrono::high_resolution_clock::now();
	InnerMapTask mapTask_GPU(numPerLine, parametersPath, inputPath);
	NTL::ZZ result = mapTask_GPU.run();
	auto finish = std::chrono::high_resolution_clock::now();
	auto totalTaskTime = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
	Json::Value root;
	std::stringstream ss;
	ss << result;
	root["result"] = ss.str();
	root["task_execution_time"] = Json::UInt64(totalTaskTime);
	root["number_additions"] = Json::UInt64(mapTask_GPU.numberAdditions);
	root["total_sum_time"] = Json::UInt64(mapTask_GPU.totalSumTime);
	root["number_multiplications"] = Json::UInt64(mapTask_GPU.numberMultiplications);
	root["total_product_time"] = Json::UInt64(mapTask_GPU.totalProductTime);
	std::ofstream results_ofs(resultsPath);
	if(results_ofs.is_open()){
		results_ofs << root;
		results_ofs.close();
	}
}
