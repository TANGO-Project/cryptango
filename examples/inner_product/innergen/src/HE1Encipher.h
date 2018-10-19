#ifndef HE1ENCIPHER_H
#define HE1ENCIPHER_H

#include <string>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>

#include "HOMEncipher.hpp"

class HE1Encipher : public HOMEncipher<NTL::ZZ_p,NTL::ZZ>
{
protected:
	NTL::ZZ_p pmod;

	void generateParameters(long lambda, long eta);
public:
	static NTL::ZZ ONE;

	std::string writeParametersToJSON() override;

	HE1Encipher();
	virtual ~HE1Encipher();
};

#endif
