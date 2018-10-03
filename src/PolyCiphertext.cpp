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

/**
 * Default constructor
 */
PolyCiphertext::PolyCiphertext() {
}

/**
 * Initialises the ciphertext with \c a
 * @param a An NTL representation of a Z[x] polynomial
 */
PolyCiphertext::PolyCiphertext(NTL::ZZX& a) : Ciphertext<NTL::ZZX>(a){
}

/**
 * Intialises the ciphertext as a degree zero polynomial (constant) with value \c a
 * @param a
 */
PolyCiphertext::PolyCiphertext(NTL::vec_ZZ& a) {
	ciphertext = to_ZZX(a);
}
/**
 * Initialises the ciphertext using the string representation of the polynomial from \c str
 * @param str
 */
PolyCiphertext::PolyCiphertext(std::string& str) {
	std::stringstream ss(str);
	ss >> ciphertext;
}

/**
 * Default destructor
 */
PolyCiphertext::~PolyCiphertext() {
}

/**
 * Write polynomial to a string. The string is a space separated list of the coefficients enclosed by square brackets.
 * @return String respresentation of the polynomials
 */
std::string PolyCiphertext::to_string(){
	std::stringstream ss;
	ss << ciphertext;
	return ss.str();
}

/**
 * Auxiliary function to compare two polynomials lexicographically (a polynomial of larger degree
 * is considered to be ordered higher than a lower degree polynomial). Returns -1 is \c a.ciphertext
 * < \c b.ciphertext, 0 if \c a.ciphertext = \c b.ciphertext, and 1 if \c a.ciphertext > \c b.ciphertext
 * @param a A \c PolyCiphertext object
 * @param b A \c PolyCiphertext object
 * @return -1, 0, or 1
 */
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

/**
 * Returns true if \c a.ciphertext is strictly less than \c b.ciphertext
 * @param a A \c PolyCiphertext object
 * @param b A \c PolyCiphertext object
 * @return \c true or \c false
 */
bool operator<(const PolyCiphertext& a, const PolyCiphertext& b){
	if(compare(a,b)==-1) return true;
	else return false;
}

/**
 * Returns true if \c a.ciphertext is less than or equal to \c b.ciphertext
 * @param a A \c PolyCiphertext object
 * @param b A \c PolyCiphertext object
 * @return \c true or \c false
 */
bool operator<=(const PolyCiphertext& a, const PolyCiphertext& b){
	if(compare(a,b)==1) return false;
	else return true;
}

/**
 * Returns true if \c a.ciphertext is equal to \c b.ciphertext
 * @param a A \c PolyCiphertext object
 * @param b A \c PolyCiphertext object
 * @return \c true or \c false
 */
bool operator==(const PolyCiphertext& a, const PolyCiphertext& b){
	if(compare(a,b)==0) return true;
	else return false;
}

/**
 * Returns true if \c a.ciphertext is greater than \c b.ciphertext
 * @param a A \c PolyCiphertext object
 * @param b A \c PolyCiphertext object
 * @return \c true or \c false
 */
bool operator>(const PolyCiphertext& a, const PolyCiphertext& b){
	if(compare(a,b)==1) return true;
	else return false;
}

/**
 * Returns true if \c a.ciphertext is greater than or equal to \c b.ciphertext
 * @param a A \c PolyCiphertext object
 * @param b A \c PolyCiphertext object
 * @return \c true or \c false
 */

bool operator>=(const PolyCiphertext& a, const PolyCiphertext& b){
	if(compare(a,b)==-1) return false;
	else return true;
}

