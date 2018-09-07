/*
 * PolyCiphertext.h
 *
 *  Created on: 27 Jun 2018
 *      Author: scsjd
 *
 *      An implementation of a polynomial OPE ciphertext. Although the ciphertext is represented by the ZZX class,
 *      this class adds lexicographical comparison and boost serialization.
 */

#ifndef _POLYCIPHERTEXT_H_
#define _POLYCIPHERTEXT_H_

#include <vector>
#include <string>
#include <NTL/ZZX.h>
#include <NTL/vec_ZZ.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "Ciphertext.hpp"

class PolyCiphertext : public Ciphertext<NTL::ZZX>{
private:
	friend int compare(const PolyCiphertext& a, const PolyCiphertext& b);

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & ciphertext;
	}
public:
	PolyCiphertext();
	PolyCiphertext(NTL::ZZX& a);
	PolyCiphertext(NTL::vec_ZZ& a);
	PolyCiphertext(std::string& str);
	virtual ~PolyCiphertext();
	std::string to_string();

	friend bool operator<(const PolyCiphertext& a, const PolyCiphertext& b);
	friend bool operator<=(const PolyCiphertext& a, const PolyCiphertext& b);
	friend bool operator==(const PolyCiphertext& a, const PolyCiphertext& b);
	friend bool operator>(const PolyCiphertext& a, const PolyCiphertext& b);
	friend bool operator>=(const PolyCiphertext& a, const PolyCiphertext& b);
};

#endif /* POLYCIPHERTEXT_H_ */
