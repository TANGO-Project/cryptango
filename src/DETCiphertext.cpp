/*
 * DETCiphertext.cpp
 *
 *  Created on: 3 Sep 2018
 *      Author: scsjd
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

