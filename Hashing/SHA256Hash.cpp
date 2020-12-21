#include <cstddef>
#include <cassert>

#include "../Utility/FileUtility.h"
#include "../Utility/BitwiseUtility.h"
#include "./SHA256Hash.h"

using namespace std;

void SHA256Hasher::PreProcess(vector<uint8_t>& buffer)
{
	size_t oldByteSize = buffer.size();
	uint64_t L = oldByteSize * 8UL;
	uint64_t restBits = L % 512;
	uint64_t minExtraBitsNeeded = 512 - restBits;
	// How many 0-bits we need for padding, should be at least 7
	uint64_t numKBits = (minExtraBitsNeeded - 1 - 64) % 512;

	buffer.resize(oldByteSize + (1 + 64 + numKBits) / 8UL);

	// Fill out the padding
	// Set the first padded bit to 1, and also already the first 7 0-bits
	size_t padCounter = 0;
	buffer[oldByteSize + padCounter++] = 0b10000000;
	numKBits -= 7;
	for (; padCounter < numKBits / 8; padCounter++)
	{
		buffer[oldByteSize + padCounter] = 0x00;
	}
	// append bit-length L as 64-bit big-endian integer
	for (uint64_t bitMask = 0x00000000000000FF; bitMask > 0; bitMask <<= 8)
	{
		// Start with last byte of little endian (most significant byte of the integer) 
		// and continue down until the first byte (least significant)
		buffer[oldByteSize + padCounter++] = static_cast<uint8_t>(L & bitMask);
	}

	// Our message should now be a multiple of 64 bytes (512 bits)
	assert(buffer.size() % 64 == 0);
}

char* SHA256Hasher::CalculateHash(const wchar_t* filePath)
{
	size_t fileSize = 0;
	vector<uint8_t> buffer = FileUtil::ReadFileContent(filePath, fileSize);
	if (fileSize == 0)
	{
		return nullptr;
	}

	// Message needs to have a length that is a multiple of 512 bits
	PreProcess(buffer);

	return nullptr;
}