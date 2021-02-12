#pragma once
#include <string>

#include "./HashingAlgorithm.h"

class MD5Hasher : public HashingAlgorithm
{
private:
    // Process and hash the message
    bool Process(const uint8_t* padding, const size_t paddingSize) override;
    // Resets the prime constants
    void ResetPrimes() override;
    // Digests the processed message and returns our hash
    std::string Digest() const override;
public:
	std::string CalculateStringHash(const std::string& input);

	inline std::string GetName() const
	{
		return "MD5";
	}

    MD5Hasher()
    {
        this->CHUNK_SIZE_BITS = 512U;
        this->CHUNK_SIZE_BYTES = 64U;
        this->MESSAGE_LENGTH_SIZE = 64U;
    }
};