#pragma once
#include <cstdint>
#include <vector>
#include <string>

#include "../Utility/FileUtility.h"

class HashingAlgorithm
{
private:
	// Start the hashing process
	std::string Hash(const size_t fileSize);
	// Pad the message so the length in bits is a multiple of chunk size, returns the padding
	virtual uint8_t* PreProcess(const size_t fileSize, size_t& paddingSize);
	// Iterate through the message in blocks and process each block
	virtual bool Process(const uint8_t* padding, const size_t paddingSize) = 0;
	// Resets the prime numbers to their initial values
	virtual void ResetPrimes() = 0;
	// Digests the processed message and returns our hash
	virtual std::string Digest() const = 0;
protected:
	FileUtil* m_pFileUtil;
	size_t m_nBytesProcessed = 0U;
	size_t CHUNK_SIZE_BYTES = 0U;
	size_t CHUNK_SIZE_BITS = 0U;
	// The size of the message length to be appended in padding, in bits
	size_t MESSAGE_LENGTH_SIZE = 0U;

	uint8_t* GetDataBlock(const size_t paddingSize, const uint8_t* padding, size_t& blockSize);
public:
	// Calculate file hash
	std::string CalculateFileHash(const wchar_t* filePath);
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
	inline size_t GetBytesProcessed() const
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