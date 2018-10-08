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

#ifndef HE2CIPHERTEXT_H_
#define HE2CIPHERTEXT_H_

#include <NTL/ZZ.h>
#include <NTL/vec_ZZ_p.h>
#include <NTL/mat_ZZ_p.h>
#include "Ciphertext.hpp"

class HE2Ciphertext : public Ciphertext<NTL::vec_ZZ_p>{
private:
	/**
	 * Public cipher parameter: the modulus for arithmetic
	 */
	static NTL::ZZ modulus;
	/**
	 * Public cipher parameter: the re-encryption matrix
	 */
	static NTL::mat_ZZ_p* R; //must be a pointer to avoid initialising with zero modulus
	friend class boost::serialization::access;
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar << modulus;
		ar << ciphertext;
	}
	template <class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar >> modulus;
		NTL::ZZ_p::init(modulus); //Need to set modulus first because >> reduces modulo p
		ar >> ciphertext;
	}
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		boost::serialization::split_free(ar,*this,version);
	}

	static NTL::vec_ZZ_p augment(const NTL::vec_ZZ_p& v);
	static NTL::vec_ZZ_p product(const NTL::vec_ZZ_p& a, const NTL::vec_ZZ_p& b);

public:
	HE2Ciphertext();
	HE2Ciphertext(NTL::vec_ZZ_p& ciphertext);
	HE2Ciphertext(NTL::vec_ZZ& ciphertext);
	HE2Ciphertext(std::string& str);
	virtual ~HE2Ciphertext();

	static void setModulus(NTL::ZZ& modulus);
	static void setMatrix(NTL::mat_ZZ_p* R);
	static void setParameters(std::string& parameters);

	HE2Ciphertext& operator+=(const HE2Ciphertext& o);
	HE2Ciphertext& operator*=(const HE2Ciphertext& o);
	friend std::ostream& operator<<(std::ostream& o, const HE2Ciphertext& c);
	friend std::istream& operator>>(std::istream& i, HE2Ciphertext& c);
	friend HE2Ciphertext operator+(const HE2Ciphertext& a, const HE2Ciphertext& b);
	friend HE2Ciphertext operator*(const HE2Ciphertext& a, const HE2Ciphertext& b);
};

#endif /* HE2CIPHERTEXT_H_ */
