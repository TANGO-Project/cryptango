/*
 * PolyCiphertext.cpp
 *
 *  Created on: 27 Jun 2018
 *      Author: scsjd
 */

#include "PolyCiphertext.h"
#include <iostream>
#include <sstream>
#include <string>

PolyCiphertext::PolyCiphertext() {
}

PolyCiphertext::PolyCiphertext(NTL::ZZX& a) : Ciphertext<NTL::ZZX>(a){
}

PolyCiphertext::PolyCiphertext(NTL::vec_ZZ& a) {
	ciphertext = to_ZZX(a);
}

PolyCiphertext::PolyCiphertext(std::string& str) {
	std::stringstream ss(str);
	ss >> ciphertext;
}

PolyCiphertext::~PolyCiphertext() {
}

std::string PolyCiphertext::to_string(){
	std::stringstream ss;
	ss << ciphertext;
	return ss.str();
}

int compare(const PolyCiphertext& a, const PolyCiphertext& b){
	//a has smaller degree than b, return -1
	if (deg(a.ciphertext) < deg(b.ciphertext)) return -1;
	//a has larger degree than b, return 1
	if (deg(a.ciphertext) > deg(b.ciphertext)) return 1;
	//same degree: need to compare coefficients
	for (int i = deg(a.ciphertext); i >=0; i--){
		if (coeff(a.ciphertext,i) < coeff(b.ciphertext,i)) return -1;
		if (coeff(a.ciphertext,i) > coeff(b.ciphertext,i)) return 1;
	}
	return 0;
}

bool operator<(const PolyCiphertext& a, const PolyCiphertext& b){
	if(compare(a,b)==-1) return true;
	else return false;
}

bool operator<=(const PolyCiphertext& a, const PolyCiphertext& b){
	if(compare(a,b)==1) return false;
	else return true;
}

bool operator==(const PolyCiphertext& a, const PolyCiphertext& b){
	if(compare(a,b)==0) return true;
	else return false;
}

bool operator>(const PolyCiphertext& a, const PolyCiphertext& b){
	if(compare(a,b)==1) return true;
	else return false;
}

bool operator>=(const PolyCiphertext& a, const PolyCiphertext& b){
	if(compare(a,b)==-1) return false;
	else return true;
}

