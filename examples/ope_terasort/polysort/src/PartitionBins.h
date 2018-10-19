/*
 * PartitionBins.h
 *
 *  Created on: 3 Jul 2018
 *      Author: scsjd
 */

#ifndef SRC_PARTITIONBINS_HPP_
#define SRC_PARTITIONBINS_HPP_

#include <map>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/map.hpp>
#include <jsoncpp/json/json.h>

#include "PolyCiphertext.h"
#include "Partition.h"

class PartitionBins{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & bins;
	}
	std::vector<Partition> bins;
public:
	PartitionBins(){};

	PartitionBins(std::size_t numPartitions){
		set_number_of_bins(numPartitions);
	};

	void set_number_of_bins(std::size_t n){
		for (std::size_t i = 0; i < n; i++){
			bins.emplace_back();
		}
	};

	Partition& get_bin(std::size_t i){
		return bins.at(i);
	};

	void add_to_bin(std::size_t i, PolyCiphertext& value){
		bins.at(i).add_to_partition(value);
	};

	std::vector<Partition>& get_bins(){
		return bins;
	};

	Json::Value to_json(){
		Json::Value root(Json::arrayValue);
		for (auto itr = bins.begin(); itr != bins.end(); itr++){
			root.append(itr->to_json());
		}
		return root;
	};

	void from_json(Json::Value json){
		for (auto itr = json.begin(); itr != json.end(); itr++){
			bins.emplace_back();
			bins.back().from_json(*itr);
		}
	};
};

#endif /* SRC_PARTITIONBINS_HPP_ */
