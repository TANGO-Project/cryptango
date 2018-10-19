/*
 * OPEReduceTask.cpp
 *
 *  Created on: 3 Jul 2018
 *      Author: scsjd
 */

#include "OPEReduceTask.h"
#include "Partition.h"
#include <fstream>
#include <algorithm>

OPEReduceTask::OPEReduceTask(const char* partitionFile, const char* outFile) {
	unsortedPartition.from_json(parse_json(partitionFile));
	this->outFile = outFile;
}

OPEReduceTask::~OPEReduceTask() {
}

void OPEReduceTask::shuffle(){
	for (int i = 0; i < unsortedPartition.size(); i++){
		sortedPartition.join(unsortedPartition[i]);
	}
	sortedPartition.sort();
}

void OPEReduceTask::run(){
	shuffle();
	std::ofstream out(outFile);
	for (int i = 0; i < sortedPartition.size(); i++){
		out << sortedPartition[i] << std::endl;
	}
	out.close();
}

Json::Value OPEReduceTask::parse_json(const char* string){
	std::ifstream ifs(string);
	Json::Value root;
	ifs >> root;
	ifs.close();
	return root;
}
