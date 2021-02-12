#include <cassert>

#include "MD5Hash.h"
#include "../Utility/FileUtility.h"

using namespace std;

constexpr size_t CHUNK_SIZE_BYTES = 64U;
constexpr size_t CHUNK_SIZE_BITS = 512U;

std::string MD5Hasher::Hash(const size_t fileSize)
{
	size_t paddingSize = 0U;
	uint8_t* padding = this->PreProcess(fileSize, paddingSize);
	if (!padding)
	{
		return "";
	}

	m_nBytesProcessed = 0U;
	if (!this->Process(padding, paddingSize))
	{
		delete[] padding;
		return "";
	}

	delete[] padding;

	string hash = this->Digest();
	this->ResetPrimes();

	return hash;
}

uint8_t* MD5Hasher::PreProcess(const size_t fileSize, size_t& paddingSize)
{
	uint64_t L = fileSize * 8UL;
	uint64_t restBits = L % CHUNK_SIZE_BITS;
	uint64_t minExtraBitsNeeded = CHUNK_SIZE_BITS - restBits;
	// How many 0-bits we need for padding, should be at least 7
	uint64_t numKBits = (minExtraBitsNeeded - 1 - 64) % CHUNK_SIZE_BITS;

	// Pad with 1 '1' bit, k 0 bits, and the length as a 64-bit integer
	paddingSize = (1 + 64 + numKBits) / 8UL;
	uint8_t* buffer = new uint8_t[paddingSize];
	if (!buffer)
	{
		return nullptr;
	}

	// Fill out the padding
	// Set the first padded bit to 1, and also already the first 7 0-bits
	size_t padCounter = 0;
	buffer[padCounter++] = 0b10000000;
	numKBits -= 7;
	for (; padCounter <= numKBits / 8; padCounter++)
	{
		buffer[padCounter] = 0x00;
	}
	// append bit-length L as 64-bit big-endian integer
	for (int c = (sizeof(L) - 1) * 8U; c >= 0; c -= 8)
	{
		// "Iterate" through L's bytes by shifting the current byte completely to the right (least significant)
		// and discarding all higher (by casting to an unsigned byte)
		buffer[padCounter++] = static_cast<uint8_t>(L >> c);
	}

	// Our message together with the padding should now be a multiple of 64 bytes (512 bits)
	assert((fileSize + paddingSize) % CHUNK_SIZE_BYTES == 0);

	return buffer;
}

bool MD5Hasher::Process(const uint8_t* padding, const size_t paddingSize)
{
	return false;
}

void MD5Hasher::ResetPrimes()
{
}

std::string MD5Hasher::Digest()
{
	return std::string();
}

std::string MD5Hasher::CalculateStringHash(const std::string& input)
{
	return "";
}