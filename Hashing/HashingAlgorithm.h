#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <string>

#include "../Utility/FileUtility.h"

class HashingAlgorithm
{
public:
	// Calculate file hash
	[[nodiscard]] std::string CalculateFileHash(const wchar_t* filePath);
	// Calculate file hash and set fileSize (not really needed, mostly for debugging purposes)
	[[nodiscard]] std::string CalculateFileHash(const wchar_t* filePath, size_t& fileSize);
	// Calculate hash for given string
	virtual std::string CalculateStringHash(const std::string& input) = 0;
	// Returns the name of the HashingAlgorithm instance, e.g. "SHA256"
	[[nodiscard]] virtual std::string GetName() const = 0;
	// Resets the algorithm's internal state, such as bytesRead and the FileUtil class
	// If a HashingAlgorithm object is used for multiple jobs, this should always be called in between
	virtual void Reset()
	{
		m_nBytesProcessed = 0U;
		m_pFileUtil->Reset();
		m_bStop = false;
	}
	// Allows us to abort the hashing process. The hash returned will then be wrong / unusable
	// If FileUtil's block size is very large it may still take some time before this takes effect.
	void Interrupt()
	{
		m_bStop = true;
	}
	[[nodiscard]] bool WasInterrupted() const
	{
		return m_bStop;
	}

	[[nodiscard]] size_t GetBytesProcessed() const
	{
		return m_nBytesProcessed;
	}
	HashingAlgorithm()
	{
		// 2^24
		m_pFileUtil = std::make_unique<FileUtil>(16777216U);
	}
	virtual ~HashingAlgorithm() = default;

protected:
	std::unique_ptr<FileUtil> m_pFileUtil;
	size_t m_nBytesProcessed = 0U;
	size_t CHUNK_SIZE_BYTES = 0U;
	size_t CHUNK_SIZE_BITS = 0U;
	// The size of the message length to be appended in padding, in bits
	size_t MESSAGE_LENGTH_SIZE = 0U;
	bool LITTLE_ENDIAN = false;
	bool m_bStop = false;

	// Caller takes ownership of data block
	[[nodiscard]] std::unique_ptr<uint8_t[]> GetDataBlock(size_t paddingSize, const std::unique_ptr<uint8_t[]>& padding, size_t& blockSize);

private:
	// Start the hashing process
	std::string Hash(size_t fileSize);
	// Pad the message so the length in bits is a multiple of chunk size, returns the padding
	virtual std::unique_ptr<uint8_t[]> PreProcess(size_t fileSize, size_t& paddingSize);
	// Iterate through the message in blocks and process each block
	virtual bool Process(std::unique_ptr<uint8_t[]> padding, size_t paddingSize) = 0;
	// Resets the prime numbers to their initial values
	virtual void ResetPrimes() = 0;
	// Digests the processed message and returns our hash
	[[nodiscard]] virtual std::string Digest() const = 0;
};