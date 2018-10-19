/*
 * HE1Array.cpp
 *
 *  Created on: 25 Jul 2018
 *      Author: scsjd
 *
 *  SIMD GPU implementation
 */

#include "HE1Array.h"

xmpIntegers_t HE1Array::modulus;
xmpHandle_t HE1Array::handle;
unsigned int HE1Array::maxPrecision;
unsigned int HE1Array::maxWords;

//Default ctor: creates new array of count ints
HE1Array::HE1Array(unsigned int count) {
	this->count = count;
	XMP_CHECK_ERROR(xmpIntegersCreate(handle, &ciphertext, maxPrecision, count));
}

HE1Array::HE1Array(int value, unsigned int count) {
	this->count = count;
	word words[maxWords * count];
	//create representation of int
	word intWord[maxWords];
	intWord[0]=(word)value;
	for (int i = 1; i < maxWords; i++) intWord[i]=0;

	//Copy to every int in array
	for (int i = 0; i < count; i++){
		memcpy(reinterpret_cast<byte*>(&words[i*maxWords]), reinterpret_cast<byte*>(intWord), maxWords*sizeof(word));
	}
	XMP_CHECK_ERROR(xmpIntegersCreate(handle, &ciphertext, maxPrecision, count));
	XMP_CHECK_ERROR(xmpIntegersImport(handle, ciphertext, maxWords, -1, sizeof(word), -1, 0, words, count));
}

HE1Array::HE1Array(NTL::ZZ& value) {
	this->count = 1;
	std::vector<word> words = ZZ_to_word_array(value,maxWords);
	XMP_CHECK_ERROR(xmpIntegersCreate(handle, &ciphertext, maxPrecision, count));
	XMP_CHECK_ERROR(xmpIntegersImport(handle, ciphertext, maxWords, -1, sizeof(word), -1, 0, words.data(), 1));
}

//Copy existing Int
HE1Array::HE1Array(xmpIntegers_t ciphertext, unsigned int count) :
		Ciphertext<xmpIntegers_t>(ciphertext) {
	this->count = count;
	this->ciphertext = ciphertext;
}

//Load array of words into Int
HE1Array::HE1Array(word* inputs, unsigned int count) {
	this->count = count;
	XMP_CHECK_ERROR(xmpIntegersCreate(handle, &ciphertext, maxPrecision, count));
	XMP_CHECK_ERROR(xmpIntegersImport(handle, ciphertext, maxWords, -1, sizeof(word), -1, 0, inputs, count));
}

//Load vector of ZZs into Int
HE1Array::HE1Array(NTL::vec_ZZ& v) {
	count = v.length();
	word inputs[maxWords * count];
	memset(reinterpret_cast<byte*>(inputs),0,maxWords*count*sizeof(word));
	for (unsigned int i = 0; i < count; i++) {
		std::vector<word> wa = ZZ_to_word_array(v.at(i),maxWords);
		memcpy(reinterpret_cast<byte*>(&inputs[i*maxWords]), reinterpret_cast<byte*>(wa.data()), maxWords*sizeof(word));
	}
	XMP_CHECK_ERROR(xmpIntegersCreate(handle, &ciphertext, maxPrecision, count));
	XMP_CHECK_ERROR(xmpIntegersImport(handle, ciphertext, maxWords, -1, sizeof(word), -1, 0, inputs, count));
}

HE1Array::~HE1Array() {
	XMP_CHECK_ERROR(xmpIntegersDestroy(handle, ciphertext));
}

unsigned int HE1Array::get_count() {
	return count;
}

void HE1Array::set_zero() {
	word words[maxWords * count];
	memset(reinterpret_cast<byte*>(words), 0, maxWords * sizeof(word) * count);
	XMP_CHECK_ERROR(xmpIntegersImport(handle, ciphertext, maxWords, -1, sizeof(word), -1, 0, words, count));
}

//Assigns prefix sum of elements in o to this
void HE1Array::prefix_sum_reduce(HE1Array& o) {
	if (this->count > 1)
		exit (cudaErrorInvalidValue);
	std::vector<word> out = o.to_host_array();
	for (int i = 0; i < o.get_count(); i++) {
		word buf[maxWords];
		std::vector<word> integerWordArray(out.begin()+i*maxWords,out.begin()+(i+1)*maxWords);
		memcpy(reinterpret_cast<byte*>(buf), reinterpret_cast<byte*>(integerWordArray.data()), maxWords*sizeof(word));
		xmpIntegers_t tmp1, tmp2;
		XMP_CHECK_ERROR(xmpIntegersCreate(handle, &tmp1, maxPrecision, 1));
		XMP_CHECK_ERROR(xmpIntegersCreate(handle, &tmp2, maxPrecision + sizeof(word) * 8, 1));
		XMP_CHECK_ERROR(xmpIntegersImport(handle, tmp1, maxWords, -1, sizeof(word), -1, 0, buf, 1));
		//Assign product of arguments to temporary
		XMP_CHECK_ERROR(xmpIntegersAdd(handle, tmp2, ciphertext, tmp1, 1));
		//Take modulus of temporary and assign to c to reduce back to precision
		XMP_CHECK_ERROR(xmpIntegersMod(handle, ciphertext, tmp2, modulus, 1));
		//Cleanup temporary
		XMP_CHECK_ERROR(xmpIntegersDestroy(handle, tmp1));
		XMP_CHECK_ERROR(xmpIntegersDestroy(handle, tmp2));
	}
}

std::vector<word> HE1Array::to_host_array() {
	word buf[maxWords * count];
	XMP_CHECK_ERROR(xmpIntegersExport(handle, buf, nullptr, -1, sizeof(word), -1, 0, ciphertext, count));
	std::vector<word> wordArray;
	wordArray.assign(buf,buf+maxWords*count);
	return wordArray;
}

std::vector<word> HE1Array::ZZ_to_word_array(NTL::ZZ& z, int numWords){
	long numBytes = NumBytes(z);
	byte bytes[numBytes];
	NTL::BytesFromZZ(bytes,z,numBytes);
	word words[numWords];
	memset(words,0,numWords*sizeof(word));
	memcpy(reinterpret_cast<byte*>(words),bytes,numBytes);
	std::vector<word> wordArray;
	wordArray.assign(words,words+numWords);
	return wordArray;
}

HE1Array& HE1Array::operator+=(HE1Array& o) {
	if (count != o.get_count())
		exit (cudaErrorInvalidValue);
	//Create a temporary int of one word larger size to store sum avoiding overflow
	xmpIntegers_t tmp;
	XMP_CHECK_ERROR(xmpIntegersCreate(handle, &tmp, maxPrecision + sizeof(word)*8, count));
	//Assign product of arguments to temporary
	XMP_CHECK_ERROR(xmpIntegersAdd(handle, tmp, ciphertext, o.ciphertext, count));
	//Take modulus of temporary and assign to c to reduce back to precision
	XMP_CHECK_ERROR(xmpIntegersMod(handle, ciphertext, tmp, modulus, count));
	//Cleanup temporary
	XMP_CHECK_ERROR(xmpIntegersDestroy(handle, tmp));
	return *this;
}

HE1Array& HE1Array::operator*=(HE1Array& o) {
	if (count != o.get_count()) exit(cudaErrorInvalidValue);
	//Create a temporary double precision int to store product avoiding overflow
	xmpIntegers_t tmp;
	XMP_CHECK_ERROR(xmpIntegersCreate(handle, &tmp, 2 * maxPrecision, count));
	//Assign product of arguments to temporary
	XMP_CHECK_ERROR(xmpIntegersMul(handle, tmp, ciphertext, o.ciphertext, count));
	//Take modulus of temporary to reduce back to precision
	XMP_CHECK_ERROR(xmpIntegersMod(handle, ciphertext, tmp, modulus, count));
	//Cleanup temporary
	XMP_CHECK_ERROR(xmpIntegersDestroy(handle, tmp));
	return *this;
}

HE1Array operator+(HE1Array& a, HE1Array& b) {
	if (a.count != b.count) exit(cudaErrorInvalidValue);
	HE1Array c(a.count);
	//Create a temporary int of one word larger size to store sum avoiding overflow
	xmpIntegers_t tmp;
	XMP_CHECK_ERROR(xmpIntegersCreate(HE1Array::handle, &tmp, HE1Array::maxPrecision + sizeof(word)*8, a.count));
	//Assign product of arguments to temporary
	XMP_CHECK_ERROR(xmpIntegersAdd(HE1Array::handle, tmp, a.ciphertext, b.ciphertext, a.count));
	//Take modulus of temporary and assign to c to reduce back to precision
	XMP_CHECK_ERROR(xmpIntegersMod(HE1Array::handle, c.ciphertext, tmp, HE1Array::modulus, a.count));
	//Cleanup temporary
	XMP_CHECK_ERROR(xmpIntegersDestroy(HE1Array::handle, tmp));
	return c;
}

HE1Array operator*(HE1Array& a, HE1Array& b) {
	if (a.count != b.count) exit(cudaErrorInvalidValue);
	HE1Array c(a.count);
	//Create a temporary double precision int to store product avoiding overflow
	xmpIntegers_t tmp;
	XMP_CHECK_ERROR(xmpIntegersCreate(HE1Array::handle, &tmp, 2 * HE1Array::maxPrecision, a.count));
	//Assign product of arguments to temporary
	XMP_CHECK_ERROR(xmpIntegersMul(HE1Array::handle, tmp, a.ciphertext, b.ciphertext, a.count));
	//Take modulus of temporary to reduce back to precision
	XMP_CHECK_ERROR(xmpIntegersMod(HE1Array::handle, c.ciphertext, tmp, HE1Array::modulus, a.count));
	//Cleanup temporary
	XMP_CHECK_ERROR(xmpIntegersDestroy(HE1Array::handle, tmp));
	return c;
}

void HE1Array::create_device_handle() {
	XMP_CHECK_ERROR(xmpHandleCreate(&handle));
}

void HE1Array::delete_device_handle() {
	XMP_CHECK_ERROR(xmpHandleDestroy(handle));
}

void HE1Array::create_device_modulus(NTL::ZZ& mod) {
	if (handle != nullptr) {
		long numBytes = NTL::NumBytes(mod);
		long numWords = numBytes / sizeof(word);
		if (numBytes % sizeof(word) != 0)
			numWords++;
		unsigned int modulus_bits = numWords * sizeof(word) * 8;
		std::vector<word> words = ZZ_to_word_array(mod,numWords);
		XMP_CHECK_ERROR(xmpIntegersCreate(handle, &modulus, modulus_bits, 1));
		XMP_CHECK_ERROR(xmpIntegersImport(handle, modulus, numWords, -1, sizeof(word), -1, 0, words.data(), 1));
		maxPrecision = modulus_bits;
		maxWords = numWords;
	} else
		throw std::logic_error("HE1Array::createDeviceModulus called before createDeviceHandle");
}

void HE1Array::delete_device_modulus() {
	XMP_CHECK_ERROR(xmpIntegersDestroy(handle, modulus));
}

NTL::vec_ZZ HE1Array::to_ZZ_vector() {
	NTL::vec_ZZ v;
	word buf[maxWords * count];
	XMP_CHECK_ERROR(xmpIntegersExport(handle, buf, nullptr, -1, sizeof(word), -1, 0, ciphertext, count));
	for (unsigned int i = 0; i < count; i++) {
		byte wa[maxWords*sizeof(word)];
		memcpy(wa, reinterpret_cast<byte*>(&buf[i*maxWords]), maxWords*sizeof(word));
		NTL::ZZ z = NTL::ZZFromBytes(wa, maxWords*sizeof(word));
		v.append(z);
	}
	return v;
}
