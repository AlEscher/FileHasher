#include <cassert>
#include <iomanip>
#include <optional>

#include "./HashingAlgorithm.h"
#include "../Utility/BitwiseUtility.h"

using namespace std;

std::string HashingAlgorithm::Hash(const size_t fileSize)
{
	size_t paddingSize = 0U;
	const auto padding = this->PreProcess(fileSize, paddingSize);
	if (!padding)
	{
		return "";
	}

	m_nBytesProcessed = 0U;
	if (!this->Process(padding.get(), paddingSize))
	{
		return "";
	}
	

	string hash = this->Digest();
	this->ResetPrimes();

	return hash;
}

unique_ptr<uint8_t[]> HashingAlgorithm::PreProcess(const size_t fileSize, size_t& paddingSize)
{
	uint64_t L = fileSize * 8UL;
	const uint64_t restBits = L % CHUNK_SIZE_BITS;
	const uint64_t minExtraBitsNeeded = CHUNK_SIZE_BITS - restBits;
	// How many 0-bits we need for padding, should be at least 7
	uint64_t numKBits = (minExtraBitsNeeded - 1 - MESSAGE_LENGTH_SIZE) % CHUNK_SIZE_BITS;

	// Pad with 1 '1' bit, k 0 bits, and the length L
	paddingSize = (1 + MESSAGE_LENGTH_SIZE + numKBits) / 8UL;
	auto buffer = make_unique<uint8_t[]>(paddingSize);
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
	const uint64_t extraPadding = std::max(MESSAGE_LENGTH_SIZE / 8 - 8, 0ULL);
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

unique_ptr<uint8_t[]> HashingAlgorithm::GetDataBlock(const size_t paddingSize, const uint8_t* padding, size_t& blockSize)
{
	size_t currentBlockSize = std::min(m_pFileUtil->BytesRemaining(), m_pFileUtil->GetBlockSize());
	unique_ptr<uint8_t[]> buffer;
	// Check if this is our last block to read, if so append our padding
	if (m_pFileUtil->BytesRemaining() <= m_pFileUtil->GetBlockSize())
	{
		const size_t lastBlockLength = m_pFileUtil->BytesRemaining();
		buffer = m_pFileUtil->GetNextBlock();
		auto finalBlock = std::make_unique<uint8_t[]>(lastBlockLength + paddingSize);
		if (!finalBlock)
		{
			return nullptr;
		}

		// Copy our block, then append the padding
		memcpy_s(finalBlock.get(), lastBlockLength + paddingSize, buffer.get(), lastBlockLength);
		memcpy_s(finalBlock.get() + lastBlockLength, lastBlockLength + paddingSize, padding, paddingSize);

		buffer = move(finalBlock);
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
	optional<size_t> fileSize = FileUtil::GetFileSizeW(filePath);
	if (!fileSize.has_value())
	{
		return "";
	}

	if (!m_pFileUtil->OpenFileStreamW(filePath) || !m_pFileUtil->IsOpen())
	{
		m_nBytesProcessed = fileSize.value();
		return "";
	}

	string hash = Hash(fileSize.value());
	if (hash.empty() || m_bStop)
	{
		m_nBytesProcessed = fileSize.value();
	}

	m_pFileUtil->Reset();
	return hash;
}

string HashingAlgorithm::CalculateFileHash(const wchar_t* filePath, size_t& fileSize)
{
	optional<size_t> fileSizeLoc = FileUtil::GetFileSizeW(filePath);
	if (!fileSizeLoc.has_value())
	{
		return "";
	}

	fileSize = fileSizeLoc.value();
	return CalculateFileHash(filePath);
}