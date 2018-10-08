/*
 * Copyright 2018 University of Leeds
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 *
 * This is being developed for the TANGO Project: http://tango-project.eu
 */
/**
 *      An implementation of a polynomial OPE ciphertext. Although the ciphertext is represented by the ZZX class,
 *      this class adds lexicographical comparison and boost serialization.
 */

#ifndef _POLYCIPHERTEXT_H_
#define _POLYCIPHERTEXT_H_

#include <vector>
#include <string>
#include <NTL/ZZX.h>
#include <NTL/vec_ZZ.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "Ciphertext.hpp"

class PolyCiphertext : public Ciphertext<NTL::ZZX>{
private:
	friend int compare(const PolyCiphertext& a, const PolyCiphertext& b);

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & ciphertext;
	}
public:
	PolyCiphertext();
	PolyCiphertext(NTL::ZZX& a);
	PolyCiphertext(NTL::vec_ZZ& a);
	PolyCiphertext(std::string& str);
	virtual ~PolyCiphertext();
	std::string to_string();

	friend bool operator<(const PolyCiphertext& a, const PolyCiphertext& b);
	friend bool operator<=(const PolyCiphertext& a, const PolyCiphertext& b);
	friend bool operator==(const PolyCiphertext& a, const PolyCiphertext& b);
	friend bool operator>(const PolyCiphertext& a, const PolyCiphertext& b);
	friend bool operator>=(const PolyCiphertext& a, const PolyCiphertext& b);
};

#endif /* POLYCIPHERTEXT_H_ */
