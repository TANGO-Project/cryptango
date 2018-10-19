/*
 * ValidateMapTask.cpp
 *
 *  Created on: 6 Jul 2018
 *      Author: scsjd
 */

#include "ValidateMapTask.h"

ValidateMapTask::ValidateMapTask(const char* secretsFile, const char* inputFile, const char* outputFile) {
	std::ifstream ifs(secretsFile, std::ios::in);
	if (ifs.is_open()){
		std::string secretstring;
		getline(ifs,secretstring);
		ifs.close();
		poly.readSecretsFromJSON(secretstring);
	}
	else throw std::ios_base::failure("Unable to open secrets file.");
	this->inputFile = inputFile;
	ofs.open(outputFile,std::ios::out);
	if (!ofs.is_open()) throw std::ios_base::failure("Unable to open output file.");
	totalDecryptionTime=0;
	numberOfInputs = 0;
}

ValidateMapTask::~ValidateMapTask() {
	ofs.close();
}

void ValidateMapTask::map(std::string& str){
	std::string key,value;
	PolyCiphertext c(str);
	NTL::ZZX cPoly = c.getCiphertext();
	auto start = std::chrono::high_resolution_clock::now();
	NTL::ZZX decPoly = poly.decrypt(cPoly);
	auto finish = std::chrono::high_resolution_clock::now();
	totalDecryptionTime += std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
	numberOfInputs++;
	PolyCiphertext decryptedPoly(decPoly);
	if (deg(lastDecryptedPoly.getCiphertext()) == -1) {
		ofs << inputFile + ":begin\t" << decryptedPoly << std::endl;
	}
	else {
		if (decryptedPoly < lastDecryptedPoly){
			ofs << "error\tmisorder in " + inputFile + " between " << lastPoly << " and " << c << std::endl;
		}
	}

	lastDecryptedPoly = decryptedPoly;
	lastPoly = c;
}

void ValidateMapTask::run(){
	std::ifstream ifs(inputFile.c_str(),std::ios::in);
	std::string line;
	while(getline(ifs,line)){
		map(line);
	}
	ifs.close();
}
