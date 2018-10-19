#ifndef HE1NENCRYPTER_H
#define HE1NENCRYPTER_H

#include <string>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/vec_ZZ.h>
#include "HE1Encipher.h"

class HE1NEncrypter : public HE1Encipher
{
private:
    NTL::ZZ kappa;
    NTL::ZZ_p kappamod;
public:
    NTL::vec_ZZ getKey();
    NTL::ZZ_p encrypt(NTL::ZZ& plaintext) override;
    std::string writeSecretsToJSON() override;

	HE1NEncrypter(int lambda, int eta, int nu);
    HE1NEncrypter(int n, int d, int rho, int rhoprime);
    HE1NEncrypter(std::string& secrets, std::string& parameters);
    virtual ~HE1NEncrypter();
};

#endif //HE1NENCRYPTER_H
