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
#include "HE1Array.h"


InnerMapTask::InnerMapTask(int numPerLine, const char* parametersPath, const char* inputPath){
	this->inputPath = inputPath;
	this->numPerLine = numPerLine;
	parseParameters(parametersPath);
	totalSumTime=0;
	numberAdditions=0;
	totalProductTime=0;
	numberMultiplications=0;
	HE1Array::create_device_handle();
	HE1Array::create_device_modulus(modulus);
}

InnerMapTask::~InnerMapTask(){
	HE1Array::delete_device_modulus();
	HE1Array::delete_device_handle();
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

	NTL::vec_ZZ v[numPerLine];

	while(getline(ifs,line)){
		std::istringstream iss(line);
		std::vector<std::string> words((std::istream_iterator<std::string>(iss)),std::istream_iterator<std::string>());
		for(int i = 0; i < words.size(); i++){
			NTL::ZZ z = NTL::conv<NTL::ZZ>(words[i].c_str());
			v[i].append(z);
		}
	}
	ifs.close();

	//Do multiplication on device
	HE1Array prod(v[0]);
	numberMultiplications = v[0].length();
	for (int i=1; i < numPerLine; i++){
		HE1Array bn(v[i]);
		auto start = std::chrono::high_resolution_clock::now();
		prod*=bn;
		auto finish = std::chrono::high_resolution_clock::now();
		totalProductTime += std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
	}

	//Do prefix sum reduce on CPU because array reduction not supported by xmp
	NTL::vec_ZZ products = prod.to_ZZ_vector();
	NTL::ZZ_p::init(modulus);
	NTL::ZZ_p result;
	for (int i = 0; i < products.length(); i++){
		auto start = std::chrono::high_resolution_clock::now();
		result += NTL::conv<NTL::ZZ_p>(products.at(i));
		auto finish = std::chrono::high_resolution_clock::now();
		totalSumTime += std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
		numberAdditions++;
	}

	return NTL::rep(result);
};


