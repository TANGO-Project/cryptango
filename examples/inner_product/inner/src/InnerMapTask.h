/*
 * InnerMapTask.h
 *
 *  Created on: 4 Jul 2018
 *      Author: scsjd
 */

#ifndef INNERMAPTASK_H_
#define INNERMAPTASK_H_

#include <NTL/ZZ.h>

class InnerMapTask {
private:
	const char* inputPath;
	const char* outputPath;
	int numPerLine;
	NTL::ZZ modulus;

	void parseParameters(const char* parametersPath);
public:
	unsigned long totalSumTime;
	unsigned long numberAdditions;
	unsigned long totalProductTime;
	unsigned long numberMultiplications;

	InnerMapTask(int numPerLine, const char* parametersPath, const char* inputPath);
	virtual ~InnerMapTask();
	NTL::ZZ run();
};

#endif /* INNERMAPTASK_H_ */
