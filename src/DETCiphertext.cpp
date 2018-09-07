/*
 * DETCiphertext.cpp
 *
 *  Created on: 3 Sep 2018
 *      Author: scsjd
 */

#include "DETCiphertext.h"

DETCiphertext::DETCiphertext() {
}

DETCiphertext::DETCiphertext(std::string& ciphertext){
	this->ciphertext=ciphertext;
}

DETCiphertext::~DETCiphertext() {
}

bool operator==(DETCiphertext& a, DETCiphertext& b){
	return (a.ciphertext==b.ciphertext);
}

