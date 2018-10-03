/*
 * HE1Ciphertext.cpp
 *
 *  Created on: 10 Jul 2018
 *      Author: scsjd
 */

#include <sstream>
#include <jsoncpp/json/json.h>

#include "HE1Ciphertext.h"

NTL::ZZ HE1Ciphertext::modulus;

/**
 * Default constructor
 */
HE1Ciphertext::HE1Ciphertext(){
}

/**
 * Initialise \c ciphertext with the supplied value
 * @param ciphertext An NTL multiprecision modular integer
 */
HE1Ciphertext::HE1Ciphertext(NTL::ZZ_p& ciphertext) : Ciphertext<NTL::ZZ_p>(ciphertext){
	NTL::ZZ_p::init(modulus);
	this->ciphertext = ciphertext;
}

/**
 * Initialise \c ciphertext with the supplied value
 * @param ciphertext An NTL multiprecision integer
 */
HE1Ciphertext::HE1Ciphertext(NTL::ZZ& ciphertext){
	NTL::ZZ_p::init(modulus);
	this->ciphertext = NTL::to_ZZ_p(ciphertext);
}

/**
 * Initialise \c ciphertext with the supplied decimal string
 * @param str A decimal string
 */
HE1Ciphertext::HE1Ciphertext(std::string& str){
	NTL::ZZ_p::init(modulus);
	this->ciphertext = NTL::to_ZZ_p(NTL::to_ZZ(str.c_str()));
}

/**
 * Default destuctor
 */
HE1Ciphertext::~HE1Ciphertext(){
}

/**
 * Sets the public modulus
 * @param mod An NTL multiprecision integer
 */
void HE1Ciphertext::setModulus(NTL::ZZ& mod){
	modulus=mod;
}

/**
 * Sets the public modulus from a JSON String
 * @param parameters A JSON string
 */
void HE1Ciphertext::setModulus(std::string& parameters){
	Json::Value root;
	Json::Reader reader;
	bool parseSuccessful = reader.parse(parameters,root);
	if (parseSuccessful){
		modulus=NTL::conv<NTL::ZZ>(root["modulus"].asCString());
	}
}

/**
 * Write \c ciphertext to output stream as decimal string
 * @param o Output stream
 * @param c \c HE1Ciphertext object
 * @return A reference to the output stream
 */
std::ostream& operator<<(std::ostream& o, const HE1Ciphertext& c){
	o << c.ciphertext;
	return o;
}

/**
 * Performs modular addition of \c a.ciphertext and \c b.ciphertext
 * @param a \c HE1Ciphertext object
 * @param b \c HE1Ciphertext object
 * @return \c HE1Ciphertext object containing the sum
 */
HE1Ciphertext operator+(const HE1Ciphertext& a, const HE1Ciphertext& b){
	HE1Ciphertext c;
	NTL::ZZ_p::init(HE1Ciphertext::modulus);
	NTL::ZZ_p tmp = a.ciphertext+b.ciphertext;
	c.set_ciphertext(tmp);
	return c;
}

/**
 * Performs modular multiplication of \c a.ciphertext and \c b.ciphertext
 * @param a \c HE1Ciphertext object
 * @param b \c HE1Ciphertext object
 * @return \c HE1Ciphertext object containing the product
 */
HE1Ciphertext operator*(const HE1Ciphertext& a, const HE1Ciphertext& b){
	HE1Ciphertext c;
	NTL::ZZ_p::init(HE1Ciphertext::modulus);
	NTL::ZZ_p tmp = a.ciphertext*b.ciphertext;
	c.set_ciphertext(tmp);
	return c;
}

/**
 * Performs modular addition of \c this.ciphertext and \c o.ciphertext. Assigns result to \c this.ciphertext
 * @param o \c HE1Ciphertext object
 * @return \c this
 */
HE1Ciphertext& HE1Ciphertext::operator+=(const HE1Ciphertext& o){
	NTL::ZZ_p::init(HE1Ciphertext::modulus);
	this->ciphertext += o.ciphertext;
	return *this;
}

/**
 * Performs modular multiplication of \c this.ciphertext and \c o.ciphertext. Assigns result to \c this.ciphertext
 * @param o \c HE1Ciphertext object
 * @return \c this
 */
HE1Ciphertext& HE1Ciphertext::operator*=(const HE1Ciphertext& o){
	NTL::ZZ_p::init(HE1Ciphertext::modulus);
	this->ciphertext *= o.ciphertext;
	return *this;
}
