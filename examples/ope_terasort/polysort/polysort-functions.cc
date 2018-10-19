/*
 * polysort-functions.cc
 *
 *  Created on: 27 Jun 2018
 *      Author: scsjd
 *
 *  Implementations of functions in project file.
 *
 */
#include <fstream>
#include <chrono>
#include "polysort.h"
#include "OPEMapTask.h"
#include "OPEReduceTask.h"

//map
void run_map_task(file splitFile, file partitionListFile, file mapOutputFile, file mapTimingsFile){
	auto start = std::chrono::high_resolution_clock::now();
	OPEMapTask mapTask(splitFile,partitionListFile, mapOutputFile);
	mapTask.run();
	auto finish = std::chrono::high_resolution_clock::now();
	auto totalTaskTime = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
	std::ofstream ofs(mapTimingsFile);
	if (ofs.is_open()){
		ofs << totalTaskTime;
		ofs.close();
	}
}
//reduce
void run_reduce_task(file partitionsFile, file outFile, file reduceTimingsFile){
	auto start = std::chrono::high_resolution_clock::now();
	OPEReduceTask reduceTask(partitionsFile,outFile);
	reduceTask.run();
	auto finish = std::chrono::high_resolution_clock::now();
	auto totalTaskTime = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
	std::ofstream ofs(reduceTimingsFile);
	if (ofs.is_open()){
		ofs << totalTaskTime;
		ofs.close();
	}
}

