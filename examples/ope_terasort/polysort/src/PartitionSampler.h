/*
 * PartitionSampling.h
 *
 *  Created on: 12 Jul 2018
 *      Author: scsjd
 */

#ifndef SRC_PARTITIONSAMPLER_H_
#define SRC_PARTITIONSAMPLER_H_

#include <string>
#include <vector>
#include "PartitionList.h"

class PartitionSampler{
private:
	std::size_t numPartitions;
	double percentSamples;
	std::vector<std::string> *inputFiles;
public:
	PartitionSampler(std::size_t numPartitions, double percentSamples, std::vector<std::string> *inputFiles);
	virtual ~PartitionSampler();
	PartitionList createPartitions();
};



#endif /* SRC_PARTITIONSAMPLER_H_ */
