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

/**
 * Default constructor. Required for \c boost serialization.
 */
SSECiphertext::SSECiphertext() {
}

/**
 * Default virtual destructor
 */
SSECiphertext::~SSECiphertext() {
}

/**
 * Intialises class members from the JSON string supplied as an argument
 * @param ciphertext JSON string
 */
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

/**
 * Decodes a hexadecimal string to a byte array
 * @param hexString hexadecimal string
 * @return byte array
 */
CryptoPP::SecByteBlock SSECiphertext::decodeHexString(std::string& hexString){
	CryptoPP::SecByteBlock decoded(nullptr,hexString.size()/2);
	CryptoPP::StringSource ss(hexString, true,
		new CryptoPP::HexDecoder(
			new CryptoPP::ArraySink(decoded, decoded.size())
		) // HexDecoder
	); // StringSource
	return decoded;
}

/**
 * Performs searching over the ciphertext by matching MAC tags. If the tag produced using the supplied key matches the
 * \c tag class member then it returns \c true, otherwise \c false
 * @param hexEncodedKey A hexadecimal encoded MAC algorithm key
 * @return \c true or \c false
 */
bool SSECiphertext::match(std::string& hexEncodedKey){
	CryptoPP::SecByteBlock key = decodeHexString(hexEncodedKey);
	return match(key);
}

/**
 * Performs searching over the ciphertext by matching MAC tags. If the tag produced using the supplied key matches the
 * \c tag class member then it returns \c true, otherwise \c false
 * @param key The MAC algorithm key as an array of bytes
 * @return \c true or \c false
 */
bool SSECiphertext::match(CryptoPP::SecByteBlock& key){
	mac.SetKey(key, key.size());
	CryptoPP::SecByteBlock searchTag(nullptr,CryptoPP::AES::BLOCKSIZE);
	CryptoPP::ArraySource(payload, payload.size(), true,
			new CryptoPP::HashFilter(mac,
					new CryptoPP::ArraySink(searchTag,searchTag.size())));
	return std::equal(tag.begin(),tag.end(),searchTag.begin());
}
