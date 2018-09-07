/*
 * SSECiphertext.cpp
 *
 *  Created on: 15 Aug 2018
 *      Author: scsjd
 */

#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <jsoncpp/json/json.h>
#include "SSECiphertext.h"

SSECiphertext::SSECiphertext() {
}

SSECiphertext::~SSECiphertext() {
	// TODO Auto-generated destructor stub
}

SSECiphertext::SSECiphertext(std::string& ciphertext){
	this->ciphertext = ciphertext;
	Json::Value root;
	Json::Reader reader;
	if(reader.parse(ciphertext, root)){
		std::string hexEncodedPayload = root["payload"].asString();
		payload = decodeHexString(hexEncodedPayload);
		std::string hexEncodedTag = root["tag"].asString();
		tag = decodeHexString(hexEncodedTag);
	}
}

CryptoPP::SecByteBlock SSECiphertext::decodeHexString(std::string& hexString){
	CryptoPP::SecByteBlock decoded(nullptr,hexString.size()/2);
	CryptoPP::StringSource ss(hexString, true,
		new CryptoPP::HexDecoder(
			new CryptoPP::ArraySink(decoded, decoded.size())
		) // HexDecoder
	); // StringSource
	return decoded;
}

bool SSECiphertext::match(std::string& hexEncodedKey){
	CryptoPP::SecByteBlock key = decodeHexString(hexEncodedKey);
	return match(key);
}

bool SSECiphertext::match(CryptoPP::SecByteBlock& key){
	mac.SetKey(key, key.size());
	CryptoPP::SecByteBlock searchTag(nullptr,CryptoPP::AES::BLOCKSIZE);
	CryptoPP::ArraySource(payload, payload.size(), true,
			new CryptoPP::HashFilter(mac,
					new CryptoPP::ArraySink(searchTag,searchTag.size())));
	return std::equal(tag.begin(),tag.end(),searchTag.begin());
}
