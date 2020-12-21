#pragma once
#include <cstdint>

namespace BitUtil
{
	// Convert a little endian 64-bit integer to big endian
	uint64_t SwapEndian64(uint64_t in);
}