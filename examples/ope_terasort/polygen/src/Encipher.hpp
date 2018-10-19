#pragma once

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
