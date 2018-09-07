/*
 * Ciphertext.h
 *
 *  Created on: 10 Jul 2018
 *      Author: scsjd
 */

#ifndef _CIPHERTEXT_H_
#define _CIPHERTEXT_H_

#include "serial.hpp"

template <typename T>
class Ciphertext {
protected:
	T ciphertext;
public:
	Ciphertext(){};
	Ciphertext(T& ciphertext){this->ciphertext=ciphertext;};
	virtual ~Ciphertext(){};
	T get_ciphertext(){return ciphertext;};
	void set_ciphertext(T& ciphertext){this->ciphertext=ciphertext;};
};

#endif /* _CIPHERTEXT_H_ */
