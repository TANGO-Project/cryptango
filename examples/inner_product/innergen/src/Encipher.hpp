/*
 * Encipher.hpp
 *
 *  Created on: 25 Jun 2018
 *      Author: James Dyer
 *
 *	Interface implemented by all enciphering classes in this library
 */

#ifndef ENCIPHER_HPP
#define ENCIPHER_HPP

#include <string>

template <typename C, typename P>
class Encipher
{
public:
	Encipher(){};
	virtual ~Encipher(){};
	virtual std::string writeSecretsToJSON()=0;
	virtual C encrypt(P& plaintext)=0;
};

#endif
