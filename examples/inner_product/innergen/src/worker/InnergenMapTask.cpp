/*
 * PolygenMapTask.cpp
 *
 *  Created on: 4 Jul 2018
 *      Author: scsjd
 */

#include "InnergenMapTask.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <iostream>

InnergenMapTask::InnergenMapTask(int rho, int d, int numLines, std::string& secrets, std::string& parameters, const char* outputPath) {
	he1n = new HE1NEncrypter(secrets, parameters);
	this->rho=rho;
	this->d=d;
	this->numLines=numLines;
	this->outputPath=outputPath;
	numberEncrypted = 0;
	totalEncryptionTime=0;
}

InnergenMapTask::~InnergenMapTask() {
	delete he1n;
}

NTL::ZZ InnergenMapTask::run(){
	std::ofstream outputFile(outputPath,std::ios::out);
	he1n->init();
	NTL::ZZ result;
	for (int i = 0; i < numLines; i++){
		std::stringstream ss;
		NTL::ZZ product(1);
		for (int j = 0; j < d; j++){
			//if(numberEncrypted == numOutputs) break;
			NTL::ZZ z = NTL::RandomBits_ZZ(rho);
			product *= z;
			auto start = std::chrono::high_resolution_clock::now();
			NTL::ZZ_p encrypted = he1n->encrypt(z);
			auto finish = std::chrono::high_resolution_clock::now();
			totalEncryptionTime += std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
			numberEncrypted++;
			ss << encrypted;
			if (j < d-1) ss << '\t';
		}
		result += product;
		outputFile << ss.str() << std::endl;
	}
	outputFile.close();
	return result;
}
