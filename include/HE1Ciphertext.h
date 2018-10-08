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

#ifndef HE1CIPHERTEXT_CPU_H_
#define HE1CIPHERTEXT_CPU_H_

#include <string>
#include <sstream>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/string.hpp>
#include "Ciphertext.hpp"


class HE1Ciphertext : public Ciphertext<NTL::ZZ_p>{
private:
	/**
	 * A public cipher parameter: the modulus for arithmetic
	 */
	static NTL::ZZ modulus;
	friend class boost::serialization::access;
	/**
	 * Serialise object to archive.
	 * @param ar An archive
	 * @param version Class version
	 */
	template <class Archive> void save(Archive & ar, const unsigned int version) const
	{
		ar << modulus;
		ar << ciphertext;
	}
	/**
	 * Deserialise object from archive
	 * @param ar An archive
	 * @param version Class version
	 */
	template <class Archive> void load(Archive & ar, const unsigned int version)
	{
		ar >> modulus;
		NTL::ZZ_p::init(modulus); //Need to set modulus first because >> reduces modulo p
		ar >> ciphertext;
	}
	/**
	 * Bidirectional serialisation of this object
	 *\see save
	 *\see load
	 * @param ar An Archive
	 * @param version Class version
	 */
	template <class Archive> void serialize(Archive & ar, const unsigned int version)
	{
		boost::serialization::split_free(ar,*this,version);
	}
public:
	HE1Ciphertext();
	HE1Ciphertext(NTL::ZZ_p& ciphertext);
	HE1Ciphertext(NTL::ZZ& ciphertext);
	HE1Ciphertext(std::string& str);
	virtual ~HE1Ciphertext();

	static void setModulus(NTL::ZZ& modulus);
	static void setModulus(std::string& parameters);

	HE1Ciphertext& operator+=(const HE1Ciphertext& o);
	HE1Ciphertext& operator*=(const HE1Ciphertext& o);
	friend std::ostream& operator<<(std::ostream& o, const HE1Ciphertext& c);
	friend std::istream& operator>>(std::istream& i, const HE1Ciphertext& c);
	friend HE1Ciphertext operator+(const HE1Ciphertext& a, const HE1Ciphertext& b);
	friend HE1Ciphertext operator*(const HE1Ciphertext& a, const HE1Ciphertext& b);
};

#endif /* HE1CIPHERTEXT_CPU_H_ */
