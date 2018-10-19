/*
 * PACDDecrypter.cpp
 *
 *  Created on: 26 Jun 2018
 *      Author: scsjd
 */

#include "PolyACDDecrypter.h"

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <NTL/vec_ZZ.h>
#include <jsoncpp/json/json.h>
#include <sstream>

PolyACDDecrypter::PolyACDDecrypter() {
}

PolyACDDecrypter::~PolyACDDecrypter() {
}

void PolyACDDecrypter::readSecretsFromJSON(std::string& json){
	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(json,root);
	if (parsingSuccessful){
		std::istringstream k1buf(root["k1"].asString());
		k1buf >> k1;
		std::istringstream k2buf(root["k2"].asString());
		k2buf >> k2;
	}
};

NTL::ZZX PolyACDDecrypter::decrypt(NTL::ZZX& ciphertext){
	NTL::ZZ c_l = LeadCoeff(ciphertext);
	NTL::ZZ m_n = c_l/k1;
	NTL::ZZX c;
	long d = deg(ciphertext)+1;
	c.SetLength(d);
	for (long i=0; i<d-1; i++){
		SetCoeff(c,i,coeff(ciphertext,i));
	}
	SetCoeff(c,d,m_n);
	SetCoeff(c,d-1,c_l-k1*m_n);
	NTL::ZZX m = c/k2;
	return m;
};
