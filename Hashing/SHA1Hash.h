#pragma once
#include "HashingAlgorithm.h"

class SHA1Hasher final : public HashingAlgorithm
{
private:
    bool Process(const uint8_t* padding, size_t paddingSize) override;
    void ResetPrimes() override;
    [[nodiscard]] std::string Digest() const override;

    uint32_t m_primes[5] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0 };
    
public:
    std::string CalculateStringHash(const std::string& input) override;

    [[nodiscard]] std::string GetName() const override
    {
        return "SHA1";
    }

    SHA1Hasher()
    {
        this->CHUNK_SIZE_BITS = 512U;
        this->CHUNK_SIZE_BYTES = 64U;
        this->MESSAGE_LENGTH_SIZE = 64U;
    }
};