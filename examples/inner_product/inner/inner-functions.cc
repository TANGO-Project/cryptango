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
