#pragma once
#include <cstdint>

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>  // Not just <immintrin.h> for compilers other than icc
#endif

#define GCC_COMPILER (defined(__GNUC__) && !defined(__clang__))

namespace BitUtil
{
	inline uint32_t SwapEndian32(uint32_t in)
	{
#if GCC_COMPILER
		return 0;
#else
		return _byteswap_ulong(in);
#endif
	}
	inline uint64_t SwapEndian64(uint64_t in)
	{
#if GCC_COMPILER
		return 0;
#else
		return _byteswap_uint64(in);
#endif
	}

	inline uint32_t rotr32(uint32_t n, int c)
	{
		return _rotr(n, c);
	}

	inline uint64_t rotr64(uint64_t n, int c)
	{
#if GCC_COMPILER
		return __rorq(n, c);
#else
		return _rotr64(n, c);
#endif
	}

	inline uint32_t rotl32(uint32_t n, int c)
	{
		return _rotl(n, c);
	}

	inline uint64_t rotl64(uint64_t n, int c)
	{
#if GCC_COMPILER
		return __rolq(n, c);
#else
		return _rotl64(n, c);
#endif
	}
	// Append one byte after the other and put them into an element of type T
	template<class T>
	T AppendBytes(const uint8_t* bytes)
	{
		T result = 0ULL;
		const size_t size = sizeof(T);
		for (size_t j = 0; j < size; j++)
		{
			// Shift first byte into upper 8 bit of our result, second byte into next 8 bits, etc...
			result |= static_cast<T>(bytes[j]) << ((size - 1 - j) * 8ULL);
		}

		return result;
	}
}
