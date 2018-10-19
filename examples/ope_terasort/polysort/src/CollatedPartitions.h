/*
 * CollatedPartitions.h
 *
 *  Created on: 3 Jul 2018
 *      Author: scsjd
 */

#ifndef SRC_COLLATEDPARTITIONS_H_
#define SRC_COLLATEDPARTITIONS_H_

#include <vector>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <jsoncpp/json/json.h>

#include "Partition.h"

class CollatedPartitions{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & partitions;
	}
	std::vector<Partition> partitions;
public:
	CollatedPartitions(){};
	virtual ~CollatedPartitions(){};

	std::vector<Partition> get_partitions(){
		return partitions;
	};

	void add_partition(Partition& p){
		partitions.push_back(p);
	};

	int size(){
		return partitions.size();
	}

	Partition& operator [](std::size_t i){
		return partitions.at(i);
	};

	Json::Value to_json(){
		Json::Value array(Json::arrayValue);
		for (auto itr = partitions.begin(); itr != partitions.end(); itr++){
			array.append(itr->to_json());
		}
		return array;
	}

	void from_json(Json::Value json){
		if (json.isArray()){
			for (auto itr = json.begin(); itr != json.end(); itr++){
				if (itr->isArray()){
					partitions.emplace_back();
					partitions.back().from_json(*itr);
				}
				else{
					std::cout << "Object serialized incorrectly";
				}
			}
		}
		else{
			std::cout << "Object serialized incorrectly";
		}
	}
};

#endif /* SRC_COLLATEDPARTITIONS_H_ */
