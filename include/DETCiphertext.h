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

#ifndef DETCIPHERTEXT_H_
#define DETCIPHERTEXT_H_

#include "Ciphertext.hpp"

class DETCiphertext: public Ciphertext<std::string> {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & ciphertext;
	}
public:
	DETCiphertext();
	DETCiphertext(std::string& ciphertext);
	virtual ~DETCiphertext();
	friend bool operator==(DETCiphertext& a, DETCiphertext& b);
};

#endif /* DETCIPHERTEXT_H_ */
