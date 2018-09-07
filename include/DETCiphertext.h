/*
 * DETCiphertext.h
 *
 *  Created on: 3 Sep 2018
 *      Author: scsjd
 */

#ifndef DETCIPHERTEXT_H_
#define DETCIPHERTEXT_H_

#include "Ciphertext.hpp"

class DETCiphertext: public Ciphertext<std::string> {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & ciphertext;
	}
public:
	DETCiphertext();
	DETCiphertext(std::string& ciphertext);
	virtual ~DETCiphertext();
	friend bool operator==(DETCiphertext& a, DETCiphertext& b);
};

#endif /* DETCIPHERTEXT_H_ */
