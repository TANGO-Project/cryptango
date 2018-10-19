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


class HE1CiphertextCPU : public Ciphertext<NTL::ZZ_p>{
private:
	static NTL::ZZ modulus;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & modulus;
	    ar & ciphertext;
	}
public:
	HE1CiphertextCPU();
	HE1CiphertextCPU(NTL::ZZ_p& ciphertext);
	HE1CiphertextCPU(NTL::ZZ& ciphertext);
	HE1CiphertextCPU(std::string& str);
	virtual ~HE1CiphertextCPU();

	static void setModulus(NTL::ZZ& modulus);
	static void setModulus(std::string& parameters);

	HE1CiphertextCPU& operator+=(const HE1CiphertextCPU& o);
	HE1CiphertextCPU& operator*=(const HE1CiphertextCPU& o);
	friend std::ostream& operator<<(std::ostream& o, const HE1CiphertextCPU& c);
	friend HE1CiphertextCPU operator+(const HE1CiphertextCPU& a, const HE1CiphertextCPU& b);
	friend HE1CiphertextCPU operator*(const HE1CiphertextCPU& a, const HE1CiphertextCPU& b);
};

#endif /* HE1CIPHERTEXT_CPU_H_ */
