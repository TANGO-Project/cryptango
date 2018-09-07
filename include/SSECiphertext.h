/*
 * SSECiphertext.h
 *
 *  Created on: 15 Aug 2018
 *      Author: scsjd
 */

#ifndef SSECIPHERTEXT_H_
#define SSECIPHERTEXT_H_

#include <cryptopp/cryptlib.h>
#include <cryptopp/aes.h>
#include <cryptopp/cmac.h>
#include <cryptopp/secblock.h>
#include "Ciphertext.hpp"

class SSECiphertext: public Ciphertext<std::string> {
private:
	CryptoPP::SecByteBlock tag;
	CryptoPP::SecByteBlock payload;
	CryptoPP::CMAC<CryptoPP::AES> mac;
	CryptoPP::SecByteBlock decodeHexString(std::string& hexString);

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & ciphertext;
		ar & tag;
		ar & payload;
	}
public:
	SSECiphertext();
	SSECiphertext(std::string& ciphertext);
	virtual ~SSECiphertext();
	bool match(std::string& hexEncodedKey);
	bool match(CryptoPP::SecByteBlock& key);
};

#endif /* SSECIPHERTEXT_H_ */
