/*
 * Partition.hpp
 *
 *  Created on: 12 Jul 2018
 *      Author: scsjd
 */

#ifndef SRC_PARTITION_HPP_
#define SRC_PARTITION_HPP_

#include <vector>
#include <algorithm>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <jsoncpp/json/json.h>

#include "PolyCiphertext.h"

class Partition{
	private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & partition;
	}
	std::vector<PolyCiphertext> partition;
public:
	Partition(){};

	std::vector<PolyCiphertext>& get_partition(){
		return partition;
	};
	void add_to_partition(PolyCiphertext& value){
		partition.push_back(value);
	};

	void join(Partition& p){
		partition.reserve(partition.size()+p.get_partition().size());
		partition.insert(partition.end(),std::make_move_iterator(p.get_partition().begin()),std::make_move_iterator(p.get_partition().end()));
		p.get_partition().clear();
		partition.shrink_to_fit();
	};

	void sort(){
		std::stable_sort(partition.begin(),partition.end());
	}

	std::size_t size(){
		return partition.size();
	}

	PolyCiphertext& operator [](std::size_t i){
		return partition.at(i);
	}

	Json::Value to_json(){
		Json::Value array(Json::arrayValue);
		for (auto itr = partition.begin(); itr != partition.end(); itr++){
			array.append(itr->to_string());
		}
		return array;
	}

	void from_json(Json::Value json){
		if (json.isArray()){
			for (auto itr = json.begin(); itr != json.end(); itr++){
				if (itr->isString()){
					std::string ctextstring = itr->asString();
					partition.emplace_back(ctextstring);
				}
				else{
					throw "PolyCiphertext serialized incorrectly: not a string!";
				}
			}
		}
		else{
			throw "Partition serialized incorrectly: not an array!";
		}
	}
};



#endif /* SRC_PARTITION_HPP_ */
