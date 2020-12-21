#include <intrin.h>

#include "./BitwiseUtility.h"

uint64_t BitUtil::SwapEndian64(uint64_t in)
{
	return _byteswap_uint64(in);
}

uint32_t BitUtil::rotr32(uint32_t n, int c)
{
	return _rotr(n, c);
}