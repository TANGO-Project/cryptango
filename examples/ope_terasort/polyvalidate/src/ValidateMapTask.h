/*
 * ValidateMapTask.h
 *
 *  Created on: 6 Jul 2018
 *      Author: scsjd
 */

#ifndef VALIDATEMAPTASK_H_
#define VALIDATEMAPTASK_H_

#include "PolyACDDecrypter.h"
#include "PolyCiphertext.h"
#include <fstream>
#include <chrono>
#include <string>

class ValidateMapTask {
private:
	PolyACDDecrypter poly;
	std::string inputFile;
	std::ofstream ofs;
	PolyCiphertext lastPoly;
	PolyCiphertext lastDecryptedPoly;
public:
	unsigned long totalDecryptionTime;
	unsigned long numberOfInputs;
	ValidateMapTask(const char* secretsFile, const char* inputFile, const char* outputFile);
	virtual ~ValidateMapTask();
	void map(std::string& str);
	void run();
};

#endif /* VALIDATEMAPTASK_H_ */
