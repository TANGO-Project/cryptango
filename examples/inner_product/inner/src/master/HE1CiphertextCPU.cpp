/*
 * HE1Ciphertext.cpp
 *
 *  Created on: 10 Jul 2018
 *      Author: scsjd
 */

#include <sstream>
#include <jsoncpp/json/json.h>

#include "HE1CiphertextCPU.h"

NTL::ZZ HE1CiphertextCPU::modulus;

HE1CiphertextCPU::HE1CiphertextCPU(){
}

HE1CiphertextCPU::HE1CiphertextCPU(NTL::ZZ_p& ciphertext) : Ciphertext<NTL::ZZ_p>(ciphertext){
	NTL::ZZ_p::init(modulus);
	this->ciphertext = ciphertext;
}

HE1CiphertextCPU::HE1CiphertextCPU(NTL::ZZ& ciphertext){
	NTL::ZZ_p::init(modulus);
	this->ciphertext = NTL::to_ZZ_p(ciphertext);
}

HE1CiphertextCPU::HE1CiphertextCPU(std::string& str){
	NTL::ZZ_p::init(modulus);
	this->ciphertext = NTL::to_ZZ_p(NTL::to_ZZ(str.c_str()));
}

HE1CiphertextCPU::~HE1CiphertextCPU(){
}

void HE1CiphertextCPU::setModulus(NTL::ZZ& mod){
	modulus=mod;
}

void HE1CiphertextCPU::setModulus(std::string& parameters){
	Json::Value root;
	Json::Reader reader;
	bool parseSuccessful = reader.parse(parameters,root);
	if (parseSuccessful){
		modulus=NTL::conv<NTL::ZZ>(root["modulus"].asCString());
	}
}


std::ostream& operator<<(std::ostream& o, const HE1CiphertextCPU& c){
	o << c.ciphertext;
	return o;
}

HE1CiphertextCPU operator+(const HE1CiphertextCPU& a, const HE1CiphertextCPU& b){
	HE1CiphertextCPU c;
	NTL::ZZ_p::init(HE1CiphertextCPU::modulus);
	NTL::ZZ_p tmp = a.ciphertext+b.ciphertext;
	c.set_ciphertext(tmp);
	return c;
}

HE1CiphertextCPU operator*(const HE1CiphertextCPU& a, const HE1CiphertextCPU& b){
	HE1CiphertextCPU c;
	NTL::ZZ_p::init(HE1CiphertextCPU::modulus);
	NTL::ZZ_p tmp = a.ciphertext*b.ciphertext;
	c.set_ciphertext(tmp);
	return c;
}

HE1CiphertextCPU& HE1CiphertextCPU::operator+=(const HE1CiphertextCPU& o){
	NTL::ZZ_p::init(HE1CiphertextCPU::modulus);
	this->ciphertext += o.ciphertext;
	return *this;
}

HE1CiphertextCPU& HE1CiphertextCPU::operator*=(const HE1CiphertextCPU& o){
	NTL::ZZ_p::init(HE1CiphertextCPU::modulus);
	this->ciphertext *= o.ciphertext;
	return *this;
}
