/*
 * PolygenMapTask.h
 *
 *  Created on: 4 Jul 2018
 *      Author: scsjd
 */

#ifndef POLYGENMAPTASK_H_
#define POLYGENMAPTASK_H_

#include <string>
#include <random>
#include <NTL/ZZX.h>
#include "PolyACDEncrypter.h"

class PolygenMapTask {
private:
	PolyACDEncrypter* poly;
	int mu, n, numOutputs;
	const char* outputPath;
	std::mt19937 rng;

	NTL::ZZX createRandomPoly();
public:
	unsigned long totalEncryptionTime;
	unsigned long numberEncrypted;

	PolygenMapTask(int mu, int n, int numOutputs, std::string& secretsJson, const char* outputPath);
	virtual ~PolygenMapTask();
	void run();
};

#endif /* POLYGENMAPTASK_H_ */
