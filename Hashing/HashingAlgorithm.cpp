#include "./HashingAlgorithm.h"

using namespace std;

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

std::string HashingAlgorithm::CalculateFileHash(const char* filePath)
{
	size_t fileSize = 0;
	fileSize = FileUtil::GetFileSizeA(filePath);
	if (fileSize == 0)
	{
		return "";
	}

	if (!m_pFileUtil->OpenFileStreamA(filePath) || !m_pFileUtil->CanRead())
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