#pragma once
#include <cstdint>
#include <vector>
#include <string>

#include "../Utility/FileUtility.h"

class HashingAlgorithm
{
protected:
	FileUtil* m_pFileUtil;
	size_t m_nBytesProcessed = 0U;
public:
	// Calculate file hash
	virtual std::string CalculateFileHash(const wchar_t* filePath) = 0;
	virtual std::string CalculateFileHash(const char* filePath) = 0;
	// Calculate file hash and set fileSize (not really needed, mostly for debugging purposes)
	virtual std::string CalculateFileHash(const wchar_t* filePath, size_t& fileSize) = 0;
	// Calculate hash for given string
	virtual std::string CalculateStringHash(const std::string& input) = 0;
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