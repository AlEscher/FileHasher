#pragma once
#include <cstdint>
#include <vector>
#include <string>

#include "../Utility/FileUtility.h"

class HashingAlgorithm
{
private:
	virtual std::string Hash(const size_t fileSize) = 0;
protected:
	FileUtil* m_pFileUtil;
	size_t m_nBytesProcessed = 0U;

	uint8_t* GetDataBlock(const size_t paddingSize, const uint8_t* padding, size_t& blockSize);
public:
	// Calculate file hash
	std::string CalculateFileHash(const wchar_t* filePath);
	std::string CalculateFileHash(const char* filePath);
	// Calculate file hash and set fileSize (not really needed, mostly for debugging purposes)
	std::string CalculateFileHash(const wchar_t* filePath, size_t& fileSize);
	// Calculate hash for given string
	virtual std::string CalculateStringHash(const std::string& input) = 0;
	// Returns the name of the HashingAlgorithm instance, e.g. "SHA256"
	virtual std::string GetName() const = 0;
	// Resets the algorithm's internal state, such as bytesRead and the FileUtil class
	virtual void Reset()
	{
		m_nBytesProcessed = 0U;
		m_pFileUtil->Reset();
	}
	virtual size_t GetBytesProcessed() const
	{
		return m_nBytesProcessed;
	}
	HashingAlgorithm()
	{
		// 2^24
		m_pFileUtil = new FileUtil(16777216U);
	}
	virtual ~HashingAlgorithm()
	{
		delete m_pFileUtil;
	}
};