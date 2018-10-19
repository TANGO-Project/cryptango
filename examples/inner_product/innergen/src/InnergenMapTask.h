/*
 * PolygenMapTask.h
 *
 *  Created on: 4 Jul 2018
 *      Author: scsjd
 */

#ifndef INNERGENMAPTASK_H_
#define INNERGENMAPTASK_H_

#include <string>
#include <random>
#include <NTL/ZZ.h>
#include "HE1NEncrypter.h"

class InnergenMapTask {
private:
	HE1NEncrypter* he1n;
	int rho, d, numLines;
	const char* outputPath;
public:
	unsigned int numberEncrypted;
	unsigned long totalEncryptionTime;
	InnergenMapTask(int rho, int d, int numOutputs, std::string& secrets, std::string& parameters, const char* outputPath);
	virtual ~InnergenMapTask();
	NTL::ZZ run();
};

#endif /* INNERGENMAPTASK_H_ */
