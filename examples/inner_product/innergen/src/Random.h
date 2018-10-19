#ifndef RANDOM_H
#define RANDOM_H

#include <NTL/ZZ.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/secblock.h>
#include "isaac.hpp"

/*
 * Implements support for uniformly distributed random arbitrary precision integers.
 * We use the ISAAC cipher as our PRNG function.
 *
 * ISAAC has been designed to be cryptographically secure if seeded properly.
 */
class Random
{
private:
	QTIsaac<>* rng;

public:
    NTL::ZZ nextBigInteger(NTL::ZZ& range);
    NTL::ZZ nextBigInteger(NTL::ZZ& min, NTL::ZZ& max);

    Random();
    Random(CryptoPP::SecByteBlock& seed);
    virtual ~Random();
};

#endif
