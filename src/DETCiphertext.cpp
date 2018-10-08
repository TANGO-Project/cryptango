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

#include "DETCiphertext.h"

/**
 * Default constructor
 */
DETCiphertext::DETCiphertext() {
}

/**
 * Initialise \c ciphertext with the supplied byte string
 * @param ciphertext A byte string
 */
DETCiphertext::DETCiphertext(std::string& ciphertext){
	this->ciphertext=ciphertext;
}

/**
 * Default destructor
 */
DETCiphertext::~DETCiphertext() {
}

/**
 * Return \c a.ciphertext = \c b.ciphertext
 * @param a A \c DETCiphertext object
 * @param b A \c DETCiphertext object
 * @return \c true or \c false
 */
bool operator==(DETCiphertext& a, DETCiphertext& b){
	return (a.ciphertext==b.ciphertext);
}

