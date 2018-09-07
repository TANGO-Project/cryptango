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

HE1Ciphertext::HE1Ciphertext(){
}

HE1Ciphertext::HE1Ciphertext(NTL::ZZ_p& ciphertext) : Ciphertext<NTL::ZZ_p>(ciphertext){
	NTL::ZZ_p::init(modulus);
	this->ciphertext = ciphertext;
}

HE1Ciphertext::HE1Ciphertext(NTL::ZZ& ciphertext){
	NTL::ZZ_p::init(modulus);
	this->ciphertext = NTL::to_ZZ_p(ciphertext);
}

HE1Ciphertext::HE1Ciphertext(std::string& str){
	NTL::ZZ_p::init(modulus);
	this->ciphertext = NTL::to_ZZ_p(NTL::to_ZZ(str.c_str()));
}

HE1Ciphertext::~HE1Ciphertext(){
}

void HE1Ciphertext::setModulus(NTL::ZZ& mod){
	modulus=mod;
}

void HE1Ciphertext::setModulus(std::string& parameters){
	Json::Value root;
	Json::Reader reader;
	bool parseSuccessful = reader.parse(parameters,root);
	if (parseSuccessful){
		modulus=NTL::conv<NTL::ZZ>(root["modulus"].asCString());
	}
}

std::ostream& operator<<(std::ostream& o, const HE1Ciphertext& c){
	o << c.ciphertext;
	return o;
}

HE1Ciphertext operator+(const HE1Ciphertext& a, const HE1Ciphertext& b){
	HE1Ciphertext c;
	NTL::ZZ_p::init(HE1Ciphertext::modulus);
	NTL::ZZ_p tmp = a.ciphertext+b.ciphertext;
	c.set_ciphertext(tmp);
	return c;
}

HE1Ciphertext operator*(const HE1Ciphertext& a, const HE1Ciphertext& b){
	HE1Ciphertext c;
	NTL::ZZ_p::init(HE1Ciphertext::modulus);
	NTL::ZZ_p tmp = a.ciphertext*b.ciphertext;
	c.set_ciphertext(tmp);
	return c;
}

HE1Ciphertext& HE1Ciphertext::operator+=(const HE1Ciphertext& o){
	NTL::ZZ_p::init(HE1Ciphertext::modulus);
	this->ciphertext += o.ciphertext;
	return *this;
}

HE1Ciphertext& HE1Ciphertext::operator*=(const HE1Ciphertext& o){
	NTL::ZZ_p::init(HE1Ciphertext::modulus);
	this->ciphertext *= o.ciphertext;
	return *this;
}
