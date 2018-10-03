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
	/**
	 * The ciphertext value encapsulated by this object
	 */
	T ciphertext;
public:
	/**
	 * Default contstructor
	 */
	Ciphertext(){};
	/**
	 * Initialise ciphertext with the value supplied
	 * @param ciphertext A ciphertext value
	 */
	Ciphertext(T& ciphertext){this->ciphertext=ciphertext;};
	/**
	 * Default destructor
	 */
	virtual ~Ciphertext(){};
	/**
	 * Accessor to return the ciphertext member
	 * @return Ciphertext value
	 */
	T get_ciphertext(){return ciphertext;};
	/**
	 * Accessor to set the ciphertext member
	 * @param ciphertext Ciphertext value
	 */
	void set_ciphertext(T& ciphertext){this->ciphertext=ciphertext;};
};

#endif /* _CIPHERTEXT_H_ */
