/*
 * PolyCiphertext.h
 *
 *  Created on: 27 Jun 2018
 *      Author: scsjd
 *
 *      An implementation of a polynomial OPE ciphertext. Although the ciphertext is represented by the ZZX class,
 *      this class adds lexicographical comparison and boost serialization.
 */

#ifndef POLYCIPHERTEXT_H_
#define POLYCIPHERTEXT_H_

#include <vector>
#include <string>
#include <sstream>
#include <NTL/ZZX.h>
#include <NTL/vec_ZZ.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/access.hpp>

#include "Ciphertext.hpp"

namespace boost{
	namespace serialization{
		template<class Archive>
		void save(Archive & ar, const NTL::ZZX& p, const unsigned int version)
		{
			std::ostringstream os;
			os << p;
			ar << os.str();
		}

		template<class Archive>
		void load(Archive & ar, NTL::ZZX& p, const unsigned int version)
		{
			std::string str;
			ar >> str;
			std::istringstream is(str);
			is >> p;
		}

		template<class Archive>
		void serialize(Archive & ar, NTL::ZZX& p, const unsigned int version)
		{
			boost::serialization::split_free(ar,p,version);
		}
	}
}

class PolyCiphertext : public Ciphertext<NTL::ZZX>{
private:
	friend int compare(const PolyCiphertext& a, const PolyCiphertext& b);

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & ciphertext;
	}
public:
	PolyCiphertext();
	PolyCiphertext(NTL::ZZX& a);
	PolyCiphertext(NTL::vec_ZZ& a);
	PolyCiphertext(std::string& str);
	virtual ~PolyCiphertext();
	std::string to_string();

	friend std::ostream& operator<<(std::ostream& os, const PolyCiphertext& c);
	friend bool operator<(const PolyCiphertext& a, const PolyCiphertext& b);
	friend bool operator<=(const PolyCiphertext& a, const PolyCiphertext& b);
	friend bool operator==(const PolyCiphertext& a, const PolyCiphertext& b);
	friend bool operator>(const PolyCiphertext& a, const PolyCiphertext& b);
	friend bool operator>=(const PolyCiphertext& a, const PolyCiphertext& b);
};

#endif /* POLYCIPHERTEXT_H_ */
