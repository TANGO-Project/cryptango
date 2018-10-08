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

#ifndef _CIPHERTEXT_H_
#define _CIPHERTEXT_H_

#include "serial.hpp"

template <typename T>
class Ciphertext {
protected:
	/**
	 * The ciphertext value encapsulated by this object
	 */
	T ciphertext;
public:
	/**
	 * Default contstructor
	 */
	Ciphertext(){};
	/**
	 * Initialise ciphertext with the value supplied
	 * @param ciphertext A ciphertext value
	 */
	Ciphertext(T& ciphertext){this->ciphertext=ciphertext;};
	/**
	 * Default destructor
	 */
	virtual ~Ciphertext(){};
	/**
	 * Accessor to return the ciphertext member
	 * @return Ciphertext value
	 */
	T get_ciphertext(){return ciphertext;};
	/**
	 * Accessor to set the ciphertext member
	 * @param ciphertext Ciphertext value
	 */
	void set_ciphertext(T& ciphertext){this->ciphertext=ciphertext;};
};

#endif /* _CIPHERTEXT_H_ */
