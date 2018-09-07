/*
 * OPECiphertext.cpp
 *
 *  Created on: 15 Aug 2018
 *      Author: scsjd
 */

#include "OPECiphertext.h"

OPECiphertext::OPECiphertext() {
	// TODO Auto-generated constructor stub

}

OPECiphertext::~OPECiphertext() {
	// TODO Auto-generated destructor stub
}

OPECiphertext::OPECiphertext(NTL::ZZ& a){
	ciphertext = a;
}

OPECiphertext::OPECiphertext(std::string& str){
	std::stringstream ss(str);
	ss >> ciphertext;
}

std::ostream& operator<<(std::ostream& o, const OPECiphertext& c){
	o << c.ciphertext;
	return o;
}

std::istream& operator>>(std::istream& i, OPECiphertext& c){
	i >> c.ciphertext;
	return i;
}

bool operator<(const OPECiphertext& a, const OPECiphertext& b){
	return a.ciphertext<b.ciphertext;
}

bool operator<=(const OPECiphertext& a, const OPECiphertext& b){
	return a.ciphertext<=b.ciphertext;
}
bool operator==(const OPECiphertext& a, const OPECiphertext& b){
	return a.ciphertext==b.ciphertext;
}
bool operator>(const OPECiphertext& a, const OPECiphertext& b){
	return a.ciphertext>b.ciphertext;
}
bool operator>=(const OPECiphertext& a, const OPECiphertext& b){
	return a.ciphertext>=b.ciphertext;
}
