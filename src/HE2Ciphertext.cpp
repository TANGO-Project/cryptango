/*
 * Copyright 2018 University of Leeds
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 *
 * This is being developed for the TANGO Project: http://tango-project.eu
 */

#include <jsoncpp/json/json.h>
#include "HE2Ciphertext.h"

NTL::ZZ HE2Ciphertext::modulus;
NTL::mat_ZZ_p* HE2Ciphertext::R;

/**
 * Default constructor
 */
HE2Ciphertext::HE2Ciphertext() {
}

/**
 * Initialize \c ciphertext with the value supplied
 * @param ciphertext A 2-vector of multiprecision integers modulo \c modulus
 */
HE2Ciphertext::HE2Ciphertext(NTL::vec_ZZ_p& ciphertext){
	this->ciphertext = ciphertext;
}

/**
 * Initialize \c ciphertext with the value supplied
 * @param ciphertext A 2-vector of multiprecision integers
 */
HE2Ciphertext::HE2Ciphertext(NTL::vec_ZZ& ciphertext){
	NTL::ZZ_p::init(modulus);
	this->ciphertext = to_vec_ZZ_p(ciphertext);
}

/**
 * Initialize \c ciphertext with the string representation of a 2-vector
 * @param str String representation of a 2-vector
 */
HE2Ciphertext::HE2Ciphertext(std::string& str){
	NTL::ZZ_p::init(modulus);
	std::stringstream ss(str);
	ss >> ciphertext;
}

/**
 * Default destructor
 */
HE2Ciphertext::~HE2Ciphertext() {
}

/**
 * Set the public modulus for arithmetic
 * @param mod NTL multiprecision integer
 */
void HE2Ciphertext::setModulus(NTL::ZZ& mod){
	modulus = mod;
}

/**
 * Set the public re-encryption matrix, \c R.
 * @param Rmatrix NTL matrix of modular integers
 */
void HE2Ciphertext::setMatrix(NTL::mat_ZZ_p* Rmatrix){
	R = Rmatrix;
}

/**
 * Set the public cipher parameters \c modulus and \c R from a JSON string.
 * @param parameters JSON string
 */
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

/**
 * Add the ciphertexts and assign to this objects \c ciphertext member
 * @param o Another \c HE2Ciphertext object
 * @return A reference to this object
 */
HE2Ciphertext& HE2Ciphertext::operator+=(const HE2Ciphertext& o){
	(*this) = (*this)+o;
	return (*this);
}

/**
 * Multiply the ciphertexts and assign to this objects \c ciphertext member.
 * In the context of the HE2 ciphers, multiplication is the Hadamard product (\see product)
 * of augmented vectors (\see augment), followed by multiplication by the matrix \c R.
 * @param o Another \c HE2Ciphertext object
 * @return A reference to this object
 */
HE2Ciphertext& HE2Ciphertext::operator*=(const HE2Ciphertext& o){
	(*this) = (*this)*o;
	return (*this);
}

/**
 * Write the \c ciphertext to a stream
 * @param o An output stream
 * @param c A \c HE2Ciphertext object
 * @return A reference to the output stream
 */
std::ostream& operator<<(std::ostream& o, const HE2Ciphertext& c){
	o << c.ciphertext;
	return o;
}

/**
 * Read the \c ciphertext from an input stream
 * @param i An input stream
 * @param c A \c HE2Ciphertext object
 * @return A reference to the output stream
 */
std::istream& operator>>(std::istream& i, HE2Ciphertext& c){
	NTL::ZZ_p::init(HE2Ciphertext::modulus);
	i >> c.ciphertext;
	return i;
}


/**
 * Add the \c ciphertext%s of each object
 * @param a An \c HE2Ciphertext object
 * @param b An \c HE2Ciphertext object
 * @return A new \c HE2Ciphertext object containing the sum
 */
HE2Ciphertext operator+(const HE2Ciphertext& a, const HE2Ciphertext& b){
	NTL::ZZ_p::init(HE2Ciphertext::modulus);
	NTL::vec_ZZ_p result = a.ciphertext+b.ciphertext;
	HE2Ciphertext c(result);
	return c;
}

/**
 * Computes the Hadamard product of two vectors
 * @param a An NTL vector of modular integers
 * @param b An NTL vector of modular integers
 * @return An NTL vector of modular integers
 */
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

/**
 * Constructs the augmented 3-vector for 2-vector \c v.
 * @param v An NTL vector of modular integers
 * @return An NTL vector of modular integers
 */
NTL::vec_ZZ_p HE2Ciphertext::augment(const NTL::vec_ZZ_p& v){
	NTL::vec_ZZ_p c;
	c.SetLength(v.length()+1);
	NTL::ZZ_p::init(HE2Ciphertext::modulus);
	for (int i =0 ; i < v.length(); i++) c[i] = v[i];
	c[v.length()]=2*v[0]-v[1];
	return c;
}

/**
 * Multiplies the \c ciphertext%s of each object. In the context of the HE2 ciphers,
 * multiplication is the Hadamard product of augmented vectors, followed by
 * multiplication by the matrix \c R.
 * \see product
 * \see augment
 * @param a An \c HE2Ciphertext object
 * @param b An \c HE2Ciphertext object
 * @return A new \c HE2Ciphertext object containing the sum
 */
HE2Ciphertext operator*(const HE2Ciphertext& a, const HE2Ciphertext& b){
	NTL::vec_ZZ_p astar = HE2Ciphertext::augment(a.ciphertext);
	NTL::vec_ZZ_p bstar = HE2Ciphertext::augment(b.ciphertext);
	NTL::vec_ZZ_p result = HE2Ciphertext::product(astar,bstar);
	NTL::ZZ_p::init(HE2Ciphertext::modulus);
	NTL::vec_ZZ_p reencrypted = (*HE2Ciphertext::R)*result;
	HE2Ciphertext c(reencrypted);
	return c;
}
