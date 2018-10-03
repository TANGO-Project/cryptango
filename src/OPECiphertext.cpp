/*
 * OPECiphertext.cpp
 *
 *  Created on: 15 Aug 2018
 *      Author: scsjd
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
