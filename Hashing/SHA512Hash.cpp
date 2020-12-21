#include <climits>
#include <string.h>

#include "./SHA512Hash.h"

// Rotate right: https://stackoverflow.com/a/776523
static inline uint32_t rotr32(uint32_t n, int c)
{
	const unsigned int mask = (CHAR_BIT * sizeof(n) - 1);
	c &= mask;
	return (n >> c) | (n << ((-c)&mask));
}

char* SHA512Hasher::CalculateHash(const wchar_t* filePath)
{
	return nullptr;
}

void SHA512Hasher::PreProcess(const char* inputBuf, size_t numBytes)
{
	uint64_t L = numBytes * 8UL;
	// How many bits we need for padding
	int numKBits = (1024 - 128 - 1 - L) % 1024;
}