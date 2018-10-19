/*
 * OPEReduceTask.h
 *
 *  Created on: 3 Jul 2018
 *      Author: scsjd
 */

#ifndef SRC_OPEREDUCETASK_H_
#define SRC_OPEREDUCETASK_H_

#include <jsoncpp/json/json.h>

#include "CollatedPartitions.h"
#include "Partition.h"

class OPEReduceTask {
	CollatedPartitions unsortedPartition;
	Partition sortedPartition;
	const char* outFile;
	void shuffle();
	Json::Value parse_json(const char* string);
public:
	OPEReduceTask(const char* partitionFile, const char* outFile);
	virtual ~OPEReduceTask();
	void run();
};

#endif /* SRC_OPEREDUCETASK_H_ */
