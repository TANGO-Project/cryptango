/*
 * Ciphertext.h
 *
 *  Created on: 10 Jul 2018
 *      Author: scsjd
 */

#ifndef CIPHERTEXT_H_
#define CIPHERTEXT_H_

#include <string>

template <typename T>
class Ciphertext {
protected:
	T ciphertext;
public:
	Ciphertext(){};
	Ciphertext(T& ciphertext){this->ciphertext=ciphertext;};
	virtual ~Ciphertext(){};
	T getCiphertext() const {return ciphertext;};
	void setCiphertext(T& ciphertext){this->ciphertext=ciphertext;};
};

#endif /* CIPHERTEXT_H_ */
