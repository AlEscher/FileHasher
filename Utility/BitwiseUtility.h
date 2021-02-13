#pragma once
#include <cstdint>

namespace BitUtil
{
	inline uint32_t SwapEndian32(uint32_t in)
	{
		return _byteswap_ulong(in);
	}
	inline uint64_t SwapEndian64(uint64_t in)
	{
		return _byteswap_uint64(in);
	}

	inline uint32_t rotr32(uint32_t n, int c)
	{
		return _rotr(n, c);
	}

	inline uint64_t rotr64(uint64_t n, int c)
	{
		return _rotr64(n, c);
	}
	// Append one byte after the other and put them into an element of type T
	template<class T>
	T AppendBytes(const uint8_t* bytes)
	{
		T result = 0ULL;
		size_t size = sizeof(T);
		for (size_t j = 0; j < size; j++)
		{
			// Shift first byte into upper 8 bit of our result, second byte into next 8 bits, etc...
			result |= static_cast<T>(bytes[j]) << ((size - 1 - j) * 8ULL);
		}

		return result;
	}
}