/*
 * HE2Ciphertext.cpp
 *
 *  Created on: 15 Aug 2018
 *      Author: scsjd
 */

#include <jsoncpp/json/json.h>
#include "HE2Ciphertext.h"

NTL::ZZ HE2Ciphertext::modulus;
NTL::mat_ZZ_p* HE2Ciphertext::R;

HE2Ciphertext::HE2Ciphertext() {
}

HE2Ciphertext::HE2Ciphertext(NTL::vec_ZZ_p& ciphertext){
	this->ciphertext = ciphertext;
}

HE2Ciphertext::HE2Ciphertext(NTL::vec_ZZ& ciphertext){
	NTL::ZZ_p::init(modulus);
	this->ciphertext = to_vec_ZZ_p(ciphertext);
}

HE2Ciphertext::HE2Ciphertext(std::string& str){
	NTL::ZZ_p::init(modulus);
	std::stringstream ss(str);
	ss >> ciphertext;
}

HE2Ciphertext::~HE2Ciphertext() {
}

void HE2Ciphertext::setModulus(NTL::ZZ& mod){
	modulus = mod;
}

void HE2Ciphertext::setMatrix(NTL::mat_ZZ_p* Rmatrix){
	R = Rmatrix;
}

void HE2Ciphertext::setParameters(std::string& parameters){
	Json::Value root;
	Json::Reader reader;
	if(reader.parse(parameters, root)){
		std::string modStr = root["modulus"].asString();
		std::istringstream mod_iss(modStr);
		mod_iss >> modulus;
		NTL::ZZ_p::init(modulus);
		R = new NTL::mat_ZZ_p;
		Json::Value RArray = root["R"];
		if(RArray.isArray()){
			int n = RArray.size();
			int m = 0;
			if(RArray[0].isArray()) m = RArray[0].size();
			else throw std::out_of_range("Row is not an array!");
			R->SetDims(n,m);
			for(unsigned int i = 0; i < 2; i++){
				Json::Value RRow = RArray[i];
				if(RRow.isArray()){
					for(unsigned int j = 0; j < 3; j++){
						if (RRow[j].isString()){
							NTL::ZZ_p elt;
							std::istringstream eltStr(RRow[j].asString());
							eltStr >> elt;
							R->put(i,j,elt);
						}
					}
				}
			}
		}
	}
}

HE2Ciphertext& HE2Ciphertext::operator+=(const HE2Ciphertext& o){
	(*this) = (*this)+o;
	return (*this);
}

HE2Ciphertext& HE2Ciphertext::operator*=(const HE2Ciphertext& o){
	(*this) = (*this)*o;
	return (*this);
}

std::ostream& operator<<(std::ostream& o, const HE2Ciphertext& c){
	o << c.ciphertext;
	return o;
}

std::istream& operator>>(std::istream& i, HE2Ciphertext& c){
	NTL::ZZ_p::init(HE2Ciphertext::modulus);
	i >> c.ciphertext;
	return i;
}

HE2Ciphertext operator+(const HE2Ciphertext& a, const HE2Ciphertext& b){
	NTL::ZZ_p::init(HE2Ciphertext::modulus);
	NTL::vec_ZZ_p result = a.ciphertext+b.ciphertext;
	HE2Ciphertext c(result);
	return c;
}

//Hadamard Product
NTL::vec_ZZ_p HE2Ciphertext::product(const NTL::vec_ZZ_p& a, const NTL::vec_ZZ_p& b){
	if (a.length()!=b.length()) throw std::out_of_range("Vectors are not equal dimension!");
	NTL::vec_ZZ_p c;
	c.SetLength(a.length());
	NTL::ZZ_p::init(HE2Ciphertext::modulus);
	for (int i =0 ; i < a.length(); i++){
		c[i] = a[i]*b[i];
	}
	return c;
}

//Creates augmented vector
NTL::vec_ZZ_p HE2Ciphertext::augment(const NTL::vec_ZZ_p& v){
	NTL::vec_ZZ_p c;
	c.SetLength(v.length()+1);
	NTL::ZZ_p::init(HE2Ciphertext::modulus);
	for (int i =0 ; i < v.length(); i++) c[i] = v[i];
	c[v.length()]=2*v[0]-v[1];
	return c;
}

HE2Ciphertext operator*(const HE2Ciphertext& a, const HE2Ciphertext& b){
	NTL::vec_ZZ_p astar = HE2Ciphertext::augment(a.ciphertext);
	NTL::vec_ZZ_p bstar = HE2Ciphertext::augment(b.ciphertext);
	NTL::vec_ZZ_p result = HE2Ciphertext::product(astar,bstar);
	NTL::ZZ_p::init(HE2Ciphertext::modulus);
	NTL::vec_ZZ_p reencrypted = (*HE2Ciphertext::R)*result;
	HE2Ciphertext c(reencrypted);
	return c;
}
