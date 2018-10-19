/*
 * PartitionList.h
 *
 *  Created on: 3 Jul 2018
 *      Author: scsjd
 */

#ifndef SRC_PARTITIONLIST_H_
#define SRC_PARTITIONLIST_H_

#include <vector>
#include <algorithm>
#include "PolyCiphertext.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <jsoncpp/json/json.h>

class PartitionList{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & list;
	}
	std::vector<PolyCiphertext> list;
public:
	PartitionList(){};

	std::vector<PolyCiphertext>& get_list(){
		return list;
	}
	void add_to_list(PolyCiphertext& value){
		list.push_back(value);
	};

	void add_to_list(std::string& value){
		list.emplace_back(value);
	};

	std::size_t size(){
		return list.size();
	}

	void sort(){
		stable_sort(list.begin(), list.end());
	}

	std::size_t get_partition(PolyCiphertext& key){
		auto pos = std::lower_bound(list.begin(),list.end(),key);
		if (pos == list.end()) return list.size();
		return pos - list.begin();
	}

	PolyCiphertext& operator [](std::size_t i){
		return list.at(i);
	}

	Json::Value to_json(){
		Json::Value array(Json::arrayValue);
		for (PolyCiphertext c : list){
			array.append(c.to_string());
		}
		return array;
	}

	void from_json(Json::Value json){
		if (json.isArray()){
			for (auto itr = json.begin(); itr != json.end(); itr++){
				if (itr->isString()){
					std::string ctextstring = itr->asString();
					add_to_list(ctextstring);
				}
				else{
					throw "PolyCiphertext serialized incorrectly: not a string!";
				}
			}
		}
		else{
			throw "PartitionList serialized incorrectly: not an array!";
		}
	}

};

#endif /* SRC_PARTITIONLIST_H_ */
