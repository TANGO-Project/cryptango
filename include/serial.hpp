/*
 * serial.hpp
 *
 *  Created on: 10 Jul 2018
 *      Author: scsjd
 */

#ifndef _SERIAL_HPP_
#define _SERIAL_HPP_

#include <sstream>
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/string.hpp>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/secblock.h>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZX.h>
#include <NTL/vec_ZZ.h>


namespace boost {
	namespace serialization {
		template<class Archive>
		void save(Archive & ar, const NTL::ZZ& p, const unsigned int version) {
			std::ostringstream oss;
			oss << p;
			ar << oss.str();
		}

		template<class Archive>
		void load(Archive & ar, NTL::ZZ& p, const unsigned int version) {
			std::string s;
			ar >> s;
			std::istringstream iss(s);
			iss >> p;
		}

		template<class Archive>
		void serialize(Archive & ar, NTL::ZZ& p, const unsigned int version) {
			boost::serialization::split_free(ar, p, version);
		}

		template<class Archive>
		void save(Archive & ar, const NTL::ZZ_p& p, const unsigned int version) {
			std::ostringstream oss;
			oss << p;
			ar << oss.str();
		}

		template<class Archive>
		void load(Archive & ar, NTL::ZZ_p& p, const unsigned int version) {
			std::string s;
			ar >> s;
			std::istringstream iss(s);
			iss >> p;
		}

		template<class Archive>
		void serialize(Archive & ar, NTL::ZZ_p& p, const unsigned int version) {
			boost::serialization::split_free(ar, p, version);
		}

		template<class Archive>
		void save(Archive & ar, const NTL::ZZX& p, const unsigned int version) {
			std::ostringstream oss;
			oss << p;
			ar << oss.str();
		}

		template<class Archive>
		void load(Archive & ar, NTL::ZZX& p, const unsigned int version) {
			std::string s;
			ar >> s;
			std::istringstream iss(s);
			iss >> p;
		}

		template<class Archive>
		void serialize(Archive & ar, NTL::ZZX& p, const unsigned int version) {
			boost::serialization::split_free(ar, p, version);
		}

		template<class Archive>
		void save(Archive & ar, const NTL::vec_ZZ& p, const unsigned int version) {
			std::ostringstream oss;
			oss << p;
			ar << oss.str();
		}

		template<class Archive>
		void load(Archive & ar, NTL::vec_ZZ& p, const unsigned int version) {
			std::string s;
			ar >> s;
			std::istringstream iss(s);
			iss >> p;
		}

		template<class Archive>
		void serialize(Archive & ar, NTL::vec_ZZ& p, const unsigned int version) {
			boost::serialization::split_free(ar, p, version);
		}

		template<class Archive>
		void save(Archive & ar, const NTL::vec_ZZ_p& p, const unsigned int version) {
			std::ostringstream oss;
			oss << p;
			ar << oss.str();
		}

		template<class Archive>
		void load(Archive & ar, NTL::vec_ZZ_p& p, const unsigned int version) {
			std::string s;
			ar >> s;
			std::istringstream iss(s);
			iss >> p;
		}

		template<class Archive>
		void serialize(Archive & ar, NTL::vec_ZZ_p& p, const unsigned int version) {
			boost::serialization::split_free(ar, p, version);
		}

		template<class Archive>
		void save(Archive & ar, const CryptoPP::SecByteBlock& p, const unsigned int version) {
			std::string encoded;
			CryptoPP::ArraySource as(p, p.size(), true,
				new CryptoPP::HexEncoder(
					new CryptoPP::StringSink(encoded)
				) // HexDecoder
			); // StringSource
			ar << encoded;
		}

		template<class Archive>
		void load(Archive & ar, CryptoPP::SecByteBlock& p, const unsigned int version) {
			std::string encoded;
			ar >> encoded;
			p.CleanNew(encoded.size()/2);
			CryptoPP::StringSource as(encoded, true,
				new CryptoPP::HexDecoder(
					new CryptoPP::ArraySink(p, p.size())
				) // HexDecoder
			); // StringSource
		}

		template<class Archive>
		void serialize(Archive & ar, CryptoPP::SecByteBlock& p, const unsigned int version) {
			boost::serialization::split_free(ar, p, version);
		}
	}
}

#endif /* _SERIAL_HPP_ */
