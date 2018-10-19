/*
 * Decipher.hpp
 *
 *  Created on: 25 Jun 2018
 *      Author: James Dyer
 *
 *	Interface implemented by all deciphering classes in this library
 */

#pragma once

#include <string>

template <typename C, typename P>
class Decipher
{
public:
	Decipher(){};
	virtual ~Decipher(){};
	virtual void readSecretsFromJSON(std::string& string)=0;
	virtual P decrypt(C& ciphertext)=0;
};
