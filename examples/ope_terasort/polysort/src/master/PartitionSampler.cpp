#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <assert.h>

#include "PartitionSampler.h"

PartitionSampler::PartitionSampler(std::size_t numPartitions, double percentSamples, std::vector<std::string> *inputFiles) {
	this->numPartitions = numPartitions;
	this->percentSamples = percentSamples;
	this->inputFiles = inputFiles;
}

PartitionSampler::~PartitionSampler() {

}

PartitionList PartitionSampler::createPartitions() {
	PartitionList keyList;

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 rng(seed);
	unsigned int randmax = rng.max();
	for (auto itr = inputFiles->begin(); itr != inputFiles->end(); itr++) {
		std::ifstream inputFile(*itr, std::ios::in);
		if (!inputFile.is_open()) {
			std::cout << "Cannot open input file " << *itr << std::endl;
			assert(false);
		}

		// Sample keys
		std::string line;
		int lineNum = 0;
		while (getline(inputFile, line)) {
			double f = (double) rng() / randmax;
			if (f < percentSamples){
				PolyCiphertext sampledKey(line);
 				keyList.add_to_list(sampledKey);
			}
			lineNum++;
		}
		inputFile.close();
	}

	// Sort sampled keys
	keyList.sort();

	// Partition keys
	PartitionList partitions;
	std::size_t sizePartition = keyList.size() / numPartitions;
	for (std::size_t i = 1; i < numPartitions; i++) {
		partitions.add_to_list(keyList[i*sizePartition]);
	}

	return partitions;
}

