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

#include "OPECiphertext.h"

/**
 * Default constructor
 */
OPECiphertext::OPECiphertext() {
	// TODO Auto-generated constructor stub

}

/**
 * Default destructor
 */
OPECiphertext::~OPECiphertext() {
	// TODO Auto-generated destructor stub
}

/**
 * Initialise \c ciphertext to the value supplied
 * @param a An NTL multiprecision integer
 */
OPECiphertext::OPECiphertext(NTL::ZZ& a){
	ciphertext = a;
}

/**
 * Initialise \c ciphertext to the value supplied
 * @param str A decimal string
 */
OPECiphertext::OPECiphertext(std::string& str){
	std::stringstream ss(str);
	ss >> ciphertext;
}

/**
 * Write \c ciphertext as decimal string to the output stream
 * @param o Output stream
 * @param c \c OPECiphertext object
 * @return a reference to the output stream
 */
std::ostream& operator<<(std::ostream& o, const OPECiphertext& c){
	o << c.ciphertext;
	return o;
}

/**
 * Read \c ciphertext as decimal string from the input stream and convert to \c NTL::ZZ
 * @param i Input stream
 * @param c \c OPECiphertext object
 * @return a reference to the output stream
 */
std::istream& operator>>(std::istream& i, OPECiphertext& c){
	i >> c.ciphertext;
	return i;
}

/**
 * Returns \c a.ciphertext \< \c b.ciphertext
 * @param a \c OPECiphertext object
 * @param b \c OPECiphertext object
 * @return \c true or \c false
 */
bool operator<(const OPECiphertext& a, const OPECiphertext& b){
	return a.ciphertext<b.ciphertext;
}

/**
 * Returns \c a.ciphertext &le; \c b.ciphertext
 * @param a \c OPECiphertext object
 * @param b \c OPECiphertext object
 * @return \c true or \c false
 */
bool operator<=(const OPECiphertext& a, const OPECiphertext& b){
	return a.ciphertext<=b.ciphertext;
}

/**
 * Returns \c a.ciphertext = \c b.ciphertext
 * @param a \c OPECiphertext object
 * @param b \c OPECiphertext object
 * @return \c true or \c false
 */
bool operator==(const OPECiphertext& a, const OPECiphertext& b){
	return a.ciphertext==b.ciphertext;
}

/**
 * Returns \c a.ciphertext \> \c b.ciphertext
 * @param a \c OPECiphertext object
 * @param b \c OPECiphertext object
 * @return \c true or \c false
 */
bool operator>(const OPECiphertext& a, const OPECiphertext& b){
	return a.ciphertext>b.ciphertext;
}

/**
 * Returns \c a.ciphertext &ge; \c b.ciphertext
 * @param a \c OPECiphertext object
 * @param b \c OPECiphertext object
 * @return \c true or \c false
 */
bool operator>=(const OPECiphertext& a, const OPECiphertext& b){
	return a.ciphertext>=b.ciphertext;
}
