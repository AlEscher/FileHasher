#include <intrin.h>

#include "./BitwiseUtility.h"

uint64_t BitUtil::SwapEndian64(uint64_t in)
{
	return _byteswap_uint64(in);
}