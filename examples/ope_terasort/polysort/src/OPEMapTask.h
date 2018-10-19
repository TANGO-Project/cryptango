/*
 * OPEMapTask.h
 *
 *  Created on: 29 Jun 2018
 *      Author: scsjd
 */

#ifndef SRC_OPEMAPTASK_H_
#define SRC_OPEMAPTASK_H_

#include <fstream>
#include <jsoncpp/json/json.h>

#include "PartitionBins.h"
#include "PolyCiphertext.h"
#include "PartitionList.h"

class OPEMapTask{
private:
	PartitionList list;
	PartitionBins bins;
	std::ifstream inputSplit;
	const char* mapOutputFile;
	PolyCiphertext key;
	bool nextKeyValue();
	void partition(PolyCiphertext& key);
	Json::Value parse_json(const char* string);
public:
	OPEMapTask(const char* splitFile, const char* partitionListFile, const char* mapOutputFile);
	virtual ~OPEMapTask();
	void run();
};

#endif /* SRC_OPEMAPTASK_H_ */
