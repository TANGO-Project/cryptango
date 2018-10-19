/*
 * InnerMapTask.cpp
 *
 *  Created on: 25 Jul 2018
 *      Author: scsjd
 */

#include <fstream>
#include <sstream>
#include <iterator>
#include <chrono>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/vec_ZZ.h>
#include <jsoncpp/json/json.h>
#include "InnerMapTask.h"
#include "HE1CiphertextCPU.h"

InnerMapTask::InnerMapTask(int numPerLine, const char* parametersPath, const char* inputPath){
	this->inputPath = inputPath;
	this->numPerLine = numPerLine;
	parseParameters(parametersPath);
	totalSumTime=0;
	numberAdditions=0;
	totalProductTime=0;
	numberMultiplications=0;
}

InnerMapTask::~InnerMapTask(){
}

void InnerMapTask::parseParameters(const char* parametersPath){
	NTL::ZZ mod;
	std::ifstream ifs(parametersPath);
	if (ifs.is_open()){
		std::string json;
		getline(ifs,json);
		Json::Value root;   // will contains the root value after parsing.
		Json::Reader reader;
		bool parsingSuccessful = reader.parse(json,root);
		if (parsingSuccessful){
			modulus = NTL::conv<NTL::ZZ>(root["modulus"].asCString());
		}
	}
}

NTL::ZZ InnerMapTask::run(){
	std::ifstream ifs(inputPath);
	if (!ifs.is_open()){
		throw std::ios_base::failure("Could not open input file.");
	}
	std::string line;

	HE1CiphertextCPU::setModulus(modulus);
	NTL::ZZ zero;
	HE1CiphertextCPU sum(zero);

	while(getline(ifs,line)){
		NTL::ZZ one;
		set(one);
		HE1CiphertextCPU prod(one);
		std::istringstream iss(line);
		std::vector<std::string> words((std::istream_iterator<std::string>(iss)),std::istream_iterator<std::string>());
		for(int i = 0; i < words.size(); i++){
			NTL::ZZ z = NTL::conv<NTL::ZZ>(words[i].c_str());
			HE1CiphertextCPU tmp(z);
			auto start = std::chrono::high_resolution_clock::now();
			prod*=tmp;
			auto finish = std::chrono::high_resolution_clock::now();
			totalProductTime += std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
			numberMultiplications++;
		}
		auto start = std::chrono::high_resolution_clock::now();
		sum+=prod;
		auto finish = std::chrono::high_resolution_clock::now();
		totalSumTime += std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
		numberAdditions++;
	}
	ifs.close();

	return NTL::rep(sum.get_ciphertext());
};


