/*
 * PolygenMapTask.cpp
 *
 *  Created on: 4 Jul 2018
 *      Author: scsjd
 */

#include "PolygenMapTask.h"
#include <fstream>
#include <chrono>
#include <iostream>

NTL::ZZX PolygenMapTask::createRandomPoly(){
	NTL::ZZX p;
	//Generate random int for degree of poly
	int rnd = (rng() % n) + 1;
	//Generate random coefficients for poly
	p.SetLength(rnd+1);
	for(int i =0; i < rnd+1; i++){
		NTL::ZZ tmp;
		RandomBits(tmp,mu);
		SetCoeff(p,i,tmp);
	};
	return p;
}

PolygenMapTask::PolygenMapTask(int mu, int n, int numOutputs, std::string& secretsJson, const char* outputPath) {
	poly = new PolyACDEncrypter(mu,secretsJson);
	this->mu=mu;
	this->n=n;
	this->numOutputs=numOutputs;
	this->outputPath=outputPath;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	rng.seed(seed);
	numberEncrypted = 0;
	totalEncryptionTime = 0;
}

PolygenMapTask::~PolygenMapTask() {
	delete poly;
}

void PolygenMapTask::run(){
	std::ofstream outputFile(outputPath,std::ios::out);
	for (int i = 0; i < numOutputs; i++){
		NTL::ZZX randomPoly = createRandomPoly();
		auto start = std::chrono::high_resolution_clock::now();
		NTL::ZZX encryptedPoly = poly->encrypt(randomPoly);
		auto finish = std::chrono::high_resolution_clock::now();
		totalEncryptionTime += std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
		numberEncrypted++;
		outputFile << encryptedPoly << std::endl;
	}
	outputFile.close();
}
