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
	/**
	 * The MAC tag of the \c payload
	 */
	CryptoPP::SecByteBlock tag;
	/**
	 * The AES-256 ciphertext
	 */
	CryptoPP::SecByteBlock payload;
	/**
	 * CMAC algorithm to generate tags
	 */
	CryptoPP::CMAC<CryptoPP::AES> mac;
	CryptoPP::SecByteBlock decodeHexString(std::string& hexString);

	/**
	 * Required for Boost serialisation
	 */
	friend class boost::serialization::access;
	/**
	 * Boost serialisation method. Serialises the object to a text or binary archive.
	 * @param ar A Boost text or binary archive
	 * @param version Version of class
	 */
	template<class Archive> void serialize(Archive & ar, const unsigned int version){
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
