/*
 * PACDEncrypter.cpp
 *
 *  Created on: 25 Jun 2018
 *      Author: scsjd
 */

#include "PolyACDEncrypter.h"

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <jsoncpp/json/json.h>
#include <string>
#include <sstream>

/*
 * \param lambda the bit length of k1
 * \param mu the size of the polynomial coefficients in bits
 * \param the degree of the polynomial k2
 */
PolyACDEncrypter::PolyACDEncrypter(int lambda, int mu, int d) {
	this->mu = mu;
	RandomLen(k1,lambda);
	k2.SetLength(d+1);
	for(int i =0; i < d; i++){
		NTL::ZZ tmp;
		RandomBits(tmp,mu);
		SetCoeff(k2,i,tmp);
	}
	SetCoeff(k2,d,1);
}

PolyACDEncrypter::PolyACDEncrypter(int mu, std::string& secrets) {
	this->mu = mu;
	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(secrets,root);
	if (parsingSuccessful){
		std::istringstream k1buf(root["k1"].asString());
		k1buf >> k1;
		std::istringstream k2buf(root["k2"].asString());
		k2buf >> k2;
	}
}

PolyACDEncrypter::~PolyACDEncrypter() {
}

/*
 * Converts the key k1 (a large integer) to a string and k2 (a polynomial) to an array of strings, then writes JSON string
 */
std::string PolyACDEncrypter::writeSecretsToJSON(){
	Json::Value root;
	std::ostringstream k1buf;
	k1buf << k1;
	root["k1"]= k1buf.str();
	std::ostringstream k2buf;
	k2buf << k2;
	root["k2"]= k2buf.str();
	Json::FastWriter writer;
	return writer.write(root);
}

NTL::ZZX PolyACDEncrypter::encrypt(NTL::ZZX& plaintext){
	NTL::ZZX p = k2*plaintext;
	long degree = deg(p);
	NTL::ZZ m_n = LeadCoeff(plaintext);
	NTL::ZZX p2;
	p2.SetLength(degree-1);
	for (int i = 0; i < degree-1; i++){
		SetCoeff(p2,i,NTL::coeff(p,i));
	}
	SetCoeff(p2,degree-1,p[degree-1]+k1*m_n);
	NTL::ZZX r;
	long d = deg(k2);
	r.SetLength(d-1);
	for(int i =0; i < d; i++){
		NTL::ZZ tmp;
		RandomBits(tmp,mu);
		SetCoeff(r,i,tmp);
	}
	return p2+r;
}

NTL::ZZX PolyACDEncrypter::encrypt(NTL::vec_ZZ& plaintext){
	NTL::ZZX ptext = to_ZZX(plaintext);
	return encrypt(ptext);
}

std::pair<NTL::ZZ,NTL::ZZX> PolyACDEncrypter::getKey(){
	return (std::pair<NTL::ZZ,NTL::ZZX>(k1,k2));
}
