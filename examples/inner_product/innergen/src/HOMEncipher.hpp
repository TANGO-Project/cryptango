#ifndef HOMENCIPHER_HPP
#define HOMENCIPHER_HPP

#include <NTL/ZZ.h>
#include "Encipher.hpp"
#include "Random.h"

template <typename C, typename P>
class HOMEncipher : public Encipher<C,P>
{
protected:
	NTL::ZZ modulus;
	NTL::ZZ p;
	NTL::ZZ q;
	Random* rng;

	void generateModulus(int lambda, int eta){
		p = NTL::RandomPrime_ZZ(lambda,20);
		q = NTL::RandomPrime_ZZ(eta,20);
		modulus = p*q;
	}

public:
	void init(){
		this->rng = new Random();
		NTL::ZZ_p::init(modulus);
	};
	NTL::ZZ getModulus(){
		return modulus;
	};
	virtual std::string writeParametersToJSON()=0;

	HOMEncipher(){
		rng=nullptr;
	};
	virtual ~HOMEncipher(){
		delete rng;
	};
};

#endif
