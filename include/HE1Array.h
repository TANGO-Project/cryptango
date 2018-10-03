/*
 * HE1Array.h
 *
 *  Created on: 25 Jul 2018
 *      Author: James Dyer
 *
 *  An implementation of HE1 for NVidia GPUs using NVLabs' xmp library.
 *  This is optimised for SIMD addition and multiplication over arrays of values.
 */

#ifndef HE1ARRAY_H_
#define HE1ARRAY_H_

#include <vector>
#include <xmp.h>
#include <NTL/ZZ.h>
#include <NTL/vec_ZZ.h>
#include "Ciphertext.hpp"

/**
 * Macro to convert the XMP error code to a user-friendly string and then exit the program with a failure code
 */
#define XMP_CHECK_ERROR(fun) \
{                             \
  xmpError_t error=fun;     \
  if(error!=xmpErrorSuccess){ \
    if(error==xmpErrorCuda)   \
      printf("CUDA Error %s, %s:%d\n",cudaGetErrorString(cudaGetLastError()),__FILE__,__LINE__); \
    else  \
      printf("XMP Error %s, %s:%d\n",xmpGetErrorString(error),__FILE__,__LINE__); \
    exit(EXIT_FAILURE); \
  } \
}

/**
 * Defines a \c byte as an unsigned 8 bit integer
 */
typedef uint8_t byte;
/**
 * Defines a \c word as an unsigned 32 bit integer
 */
typedef uint32_t word;

class HE1Array: public Ciphertext<xmpIntegers_t> {
private:
	/**
	 * Contains the modulus for modular arithmetic on the device
	 */
	static xmpIntegers_t modulus;
	/**
	 * The XMP device context handle
	 */
	static xmpHandle_t handle;
	/**
	 * The maximum precision for \c ciphertext
	 */
	static unsigned int maxPrecision;
	/**
	 * The maximum number of words in \c ciphertext
	 */
	static unsigned int maxWords;

	/**
	 * Number of integers stored in ciphertext array
	 */
	unsigned int count;

	static std::vector<word> ZZ_to_word_array(NTL::ZZ& z, int numWords);
public:
	HE1Array(unsigned int count);
	HE1Array(int value, unsigned int count);
	HE1Array(NTL::ZZ& value);
	HE1Array(xmpIntegers_t ciphertext, unsigned int count);
	HE1Array(word* inputs, unsigned int count);
	HE1Array(NTL::vec_ZZ& v);
	virtual ~HE1Array();

	void set_zero();
	unsigned int get_count();
	NTL::vec_ZZ to_ZZ_vector();
	std::vector<word> to_host_array();
	HE1Array& operator*=(HE1Array& o);
	HE1Array& operator+=(HE1Array& o);

	friend HE1Array operator*(HE1Array& a, HE1Array& b);
	friend HE1Array operator+(HE1Array& a, HE1Array& b);

	static void create_device_handle();
	static void delete_device_handle();
	static void create_device_modulus(NTL::ZZ& modulus);
	static void delete_device_modulus();
};

#endif /* HE1ARRAY_H_ */
