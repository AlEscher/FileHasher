#pragma once
#include <cstdint>

namespace BitUtil
{
	// Convert a little endian 64-bit integer to big endian
	uint64_t SwapEndian64(uint64_t in);
	uint32_t rotr32(uint32_t n, int c);
	uint64_t rotr64(uint64_t n, int c);
}