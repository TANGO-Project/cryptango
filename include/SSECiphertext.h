/*
 * Copyright 2018 University of Leeds
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 *
 * This is being developed for the TANGO Project: http://tango-project.eu
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
