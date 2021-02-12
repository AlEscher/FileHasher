#pragma once
#include <string>

#include "./HashingAlgorithm.h"

class MD5Hasher : public HashingAlgorithm
{
private:
    std::string Hash(const size_t fileSize) override;
    // Pad the message so the length in bits is a multiple of 512, returns the padding
    uint8_t* PreProcess(const size_t fileSize, size_t& paddingSize);
    // Process and hash the message
    bool Process(const uint8_t* padding, const size_t paddingSize);
    // Resets the prime constants
    void ResetPrimes();
    // Digests the processed message and returns our hash
    std::string Digest();
public:
	std::string CalculateStringHash(const std::string& input);

	inline std::string GetName() const
	{
		return "MD5";
	}
};