# cryptango
&copy; James Dyer and University of Leeds 2018

Ciphertext classes for European Project TANGO (http://tango-project.eu).
## Description
This C++ library provides ciphertext types to allow computation over encrypted data in TANGO. Each class (except HE1Array) supports Boost serialization, as is required by the TANGO programming model. The classes are:
- DETCiphertext: ciphertext encrypted with a deterministic cipher. Supports equality tests.
- HE1Ciphertext: ciphertext encrypted with HE1/N homomorphic ciphers. Supports addition and multiplication.
- HE2Ciphertext: ciphertext encrypted with HE2/N homomorphic ciphers. Supports addition and multiplication.
- HE1Array: ciphertext encrypted with HE1/N homomorphic ciphers. Implementation for GPU addition and multiplication.
- OPECiphertext: ciphertext encrypted with GACD OPE cipher. Supports comparison tests.
- PolyCiphertext: ciphertext encrypted with PolyACD OPE cipher. Supports comparison tests.
- SSECiphertext: ciphertext encrypted with SSE cipher. Supports equality test (`match`).

## Compilation
To compile:

```shell
cd cryptango
make
```
This builds a static and shared library in the `lib/` subfolder. You may need to edit the `LDFLAGS` make variable in `Makefile` to target the correct system library directories for your flavour of Linux. The entries given are correct for Ubuntu. If you don't require GPU support, remove `HE1Array.cpp` from `src/` and compile.

### Requirements
- gcc 4.8+ 
- boost: `boost_iostreams` and `boost_serialization` libraries (https://www.boost.org/)
- NTL: Number Theory Library (http://www.shoup.net/ntl/)
- jsoncpp: JSON C++ library (https://github.com/open-source-parsers/jsoncpp)
- Crypto++: C++ cryptographic library (https://www.cryptopp.com/)

Additionally, for GPU support
- xmp (https://github.com/NVlabs/xmp/)
- CUDA (https://developer.nvidia.com/cuda-zone)
