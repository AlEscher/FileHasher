
#include "./SHA512Hash.h"


std::string SHA512Hasher::CalculateHash(const wchar_t* filePath)
{
	return nullptr;
}

void SHA512Hasher::PreProcess(const char* inputBuf, size_t numBytes)
{
	uint64_t L = numBytes * 8UL;
	// How many bits we need for padding
	int numKBits = (1024 - 128 - 1 - L) % 1024;
}