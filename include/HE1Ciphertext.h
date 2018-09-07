/*
 * HE1Ciphertext.h
 *
 *  Created on: 10 Jul 2018
 *      Author: scsjd
 *
 *      CPU based implementation of HE1 addition and multiplication algorithms
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
	static NTL::ZZ modulus;
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
