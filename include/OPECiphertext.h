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
 *  Most of the functionality of this class is already encapsulated in the
 *  NTL::ZZ type. However, this class implements boost serialization
 *  (required for the programming model) which NTL::ZZ does not.
 */
#ifndef OPECIPHERTEXT_H_
#define OPECIPHERTEXT_H_

#include "Ciphertext.hpp"

class OPECiphertext: public Ciphertext<NTL::ZZ> {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & ciphertext;
	}
public:
	OPECiphertext();
	OPECiphertext(NTL::ZZ& a);
	OPECiphertext(NTL::vec_ZZ& a);
	OPECiphertext(std::string& str);
	virtual ~OPECiphertext();

	friend std::ostream& operator<<(std::ostream& o, const OPECiphertext& c);
	friend std::istream& operator>>(std::istream& i, OPECiphertext& c);
	friend bool operator<(const OPECiphertext& a, const OPECiphertext& b);
	friend bool operator<=(const OPECiphertext& a, const OPECiphertext& b);
	friend bool operator==(const OPECiphertext& a, const OPECiphertext& b);
	friend bool operator>(const OPECiphertext& a, const OPECiphertext& b);
	friend bool operator>=(const OPECiphertext& a, const OPECiphertext& b);};

#endif /* OPECIPHERTEXT_H_ */
