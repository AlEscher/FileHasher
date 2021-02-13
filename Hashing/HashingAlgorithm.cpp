#include <sstream>
#include <iomanip>
#include <cassert>

#include "./HashingAlgorithm.h"
#include "../Utility/BitwiseUtility.h"

using namespace std;

std::string HashingAlgorithm::Hash(const size_t fileSize)
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

uint8_t* HashingAlgorithm::PreProcess(const size_t fileSize, size_t& paddingSize)
{
	uint64_t L = fileSize * 8UL;
	uint64_t restBits = L % CHUNK_SIZE_BITS;
	uint64_t minExtraBitsNeeded = CHUNK_SIZE_BITS - restBits;
	// How many 0-bits we need for padding, should be at least 7
	uint64_t numKBits = (minExtraBitsNeeded - 1 - MESSAGE_LENGTH_SIZE) % CHUNK_SIZE_BITS;

	// Pad with 1 '1' bit, k 0 bits, and the length L
	paddingSize = (1 + MESSAGE_LENGTH_SIZE + numKBits) / 8UL;
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
	// Extra 8 bytes since for SHA512 L must be 128 bits, upper 64 bits will always be 0
	// For e.g. SHA256 where L must be 64 bits, this will be 0
	uint64_t extraPadding = std::max(MESSAGE_LENGTH_SIZE / 8 - 8, 0ULL);
	for (; padCounter <= numKBits / 8 + extraPadding; padCounter++)
	{
		buffer[padCounter] = 0x00;
	}
	if (LITTLE_ENDIAN)
	{
		L = BitUtil::SwapEndian64(L);
	}
	// append bit-length L
	for (int c = (sizeof(L) - 1) * 8U; c >= 0; c -= 8)
	{
		// "Iterate" through L's bytes by shifting the current byte completely to the right (least significant)
		// and discarding all higher (by casting to an unsigned byte)
		buffer[padCounter++] = static_cast<uint8_t>(L >> c);
	}

	// Our message together with the padding should now be a multiple of the block size
	assert((fileSize + paddingSize) % CHUNK_SIZE_BYTES == 0);

	return buffer;
}

uint8_t* HashingAlgorithm::GetDataBlock(const size_t paddingSize, const uint8_t* padding, size_t& blockSize)
{
	size_t currentBlockSize = std::min(m_pFileUtil->BytesRemaining(), m_pFileUtil->GetBlockSize());
	uint8_t* buffer = nullptr;
	// Check if this is our last block to read, if so append our padding
	if (m_pFileUtil->BytesRemaining() <= m_pFileUtil->GetBlockSize())
	{
		size_t lastBlockLength = m_pFileUtil->BytesRemaining();
		buffer = m_pFileUtil->GetNextBlock();
		uint8_t* finalBlock = new uint8_t[lastBlockLength + paddingSize];
		if (!finalBlock)
		{
			delete[] buffer;
			return nullptr;
		}

		// Copy our block, then append the padding
		memcpy_s(finalBlock, lastBlockLength + paddingSize, buffer, lastBlockLength);
		memcpy_s(finalBlock + lastBlockLength, lastBlockLength + paddingSize, padding, paddingSize);

		delete[] buffer;
		buffer = finalBlock;
		currentBlockSize = lastBlockLength + paddingSize;
	}
	else
	{
		buffer = m_pFileUtil->GetNextBlock();
	}

	blockSize = currentBlockSize;
	m_nBytesProcessed += blockSize;

	return buffer;
}

string HashingAlgorithm::CalculateFileHash(const wchar_t* filePath)
{
	size_t fileSize = 0;
	fileSize = FileUtil::GetFileSizeW(filePath);
	if (fileSize == 0)
	{
		return "";
	}

	if (!m_pFileUtil->OpenFileStreamW(filePath) || !m_pFileUtil->CanRead())
	{
		return "";
	}

	string hash = Hash(fileSize);
	m_pFileUtil->Reset();
	return hash;
}

string HashingAlgorithm::CalculateFileHash(const wchar_t* filePath, size_t& fileSize)
{
	size_t fileSizeLoc = fileSize = 0;
	fileSizeLoc = FileUtil::GetFileSizeW(filePath);
	if (fileSizeLoc == 0)
	{
		return "";
	}

	fileSize = fileSizeLoc;
	return CalculateFileHash(filePath);
}