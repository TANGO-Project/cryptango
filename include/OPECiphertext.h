/*
 * OPECiphertext.h
 *
 *  Created on: 15 Aug 2018
 *      Author: scsjd
 *
 *  Most of the functionality of this class is already encapsulated in the
 *  NTL::ZZ type. However, this class implements boost serialization
 *  (required for the programming model) which NTL::ZZ does not.
 */

#ifndef OPECIPHERTEXT_H_
#define OPECIPHERTEXT_H_

#include "Ciphertext.hpp"

class OPECiphertext: public Ciphertext<NTL::ZZ> {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & ciphertext;
	}
public:
	OPECiphertext();
	OPECiphertext(NTL::ZZ& a);
	OPECiphertext(NTL::vec_ZZ& a);
	OPECiphertext(std::string& str);
	virtual ~OPECiphertext();

	friend std::ostream& operator<<(std::ostream& o, const OPECiphertext& c);
	friend std::istream& operator>>(std::istream& i, OPECiphertext& c);
	friend bool operator<(const OPECiphertext& a, const OPECiphertext& b);
	friend bool operator<=(const OPECiphertext& a, const OPECiphertext& b);
	friend bool operator==(const OPECiphertext& a, const OPECiphertext& b);
	friend bool operator>(const OPECiphertext& a, const OPECiphertext& b);
	friend bool operator>=(const OPECiphertext& a, const OPECiphertext& b);};

#endif /* OPECIPHERTEXT_H_ */
