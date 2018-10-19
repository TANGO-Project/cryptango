/*
 * OPEMapTask.cpp
 *
 *  Created on: 29 Jun 2018
 *      Author: scsjd
 */

#include "OPEMapTask.h"
#include <iostream>

OPEMapTask::OPEMapTask(const char* splitFile, const char* partitionListFile, const char* mapOutputFile){
	list.from_json(parse_json(partitionListFile));
	bins.set_number_of_bins(list.size()+1);
	this->mapOutputFile = mapOutputFile;
	inputSplit.open(splitFile,std::ios::in);
}

OPEMapTask::~OPEMapTask() {
	inputSplit.close();
}

bool OPEMapTask::nextKeyValue(){
	std::string line;
	if (std::getline(inputSplit,line)){
		PolyCiphertext c(line);
		key = c;
		return true;
	}
	else return false;
}

void OPEMapTask::partition(PolyCiphertext& key){
	std::size_t index = list.get_partition(key);
	bins.get_bin(index).add_to_partition(key);
}

void OPEMapTask::run(){
	//parse input as key-value pair and map
	while (nextKeyValue()){
		partition(key);
	}
	std::ofstream mapOutput(mapOutputFile);
	mapOutput << bins.to_json();
	mapOutput.close();
}

Json::Value OPEMapTask::parse_json(const char* filename){
	Json::Value root;
	std::ifstream ifs(filename);
	std::string errs;
	if(ifs.is_open()){
		Json::CharReaderBuilder rbuilder;
		if(!Json::parseFromStream(rbuilder,ifs,&root,&errs)) throw errs;
		ifs.close();
	}
	else{
		errs = "Cannot open file: " + std::string(filename);
		throw errs;
	}
	return root;
}
