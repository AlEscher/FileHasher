#include <sstream>
#include <iomanip>

#include "SHA1Hash.h"
#include "../Utility/FileUtility.h"
#include "../Utility/BitwiseUtility.h"

using namespace std;

constexpr size_t ENTRY_MESSAGE_SIZE = 80U;

bool SHA1Hasher::Process(const uint8_t* padding, const size_t paddingSize)
{
	do
	{
		size_t currentBlockSize = 0U;
		uint8_t* buffer = GetDataBlock(paddingSize, padding, currentBlockSize);
		if (!buffer)
		{
			return false;
		}

		// Break one block of the message into 512-bit chunks
		for (size_t chunk = 0; chunk < currentBlockSize; chunk += CHUNK_SIZE_BYTES)
		{
			uint32_t M[ENTRY_MESSAGE_SIZE]{ 0 };

			// Load bytes of this chunk into our entry message array
			for (size_t i = 0; i < 16; i++)
			{
				// Append 4 bytes and put them into one variable
				M[i] = BitUtil::AppendBytes<uint32_t>(buffer + chunk + (i * 4));
			}

			for (size_t i = 16; i < ENTRY_MESSAGE_SIZE; i++)
			{
				M[i] = BitUtil::rotl32((M[i - 3] ^ M[i - 8] ^ M[i - 14] ^ M[i - 16]), 1);
			}

			uint32_t A = m_Primes[0], B = m_Primes[1], C = m_Primes[2], D = m_Primes[3], E = m_Primes[4];

			for (size_t i = 0; i < ENTRY_MESSAGE_SIZE; i++)
			{
				uint32_t F = 0U;
				uint32_t k = 0U;

				if (i < 20)
				{
					F = (B & C) | (~B & D);
					k = 0x5a827999;
				}
				else if (20 <= i && i < 40)
				{
					F = B ^ C ^ D;
					k = 0x6ed9eba1;
				}
				else if (40 <= i && i < 60)
				{
					F = (B & C) | (B & D) | (C & D);
					k = 0x8f1bbcdc;
				}
				else if (60 <= i && i < 80)
				{
					F = B ^ C ^ D;
					k = 0xca62c1d6;
				}

				const uint32_t temp = BitUtil::rotl32(A, 5) + F + E + k + M[i];
				E = D; D = C;
				C = BitUtil::rotl32(B, 30); B = A; A = temp;
			}

			m_Primes[0] += A; m_Primes[1] += B; m_Primes[2] += C; m_Primes[3] += D; m_Primes[4] += E;
		}

		delete[] buffer;
	} while (m_pFileUtil->CanRead());

	return true;
}

void SHA1Hasher::ResetPrimes()
{
	m_Primes[0] = 0x67452301;
	m_Primes[1] = 0xefcdab89;
	m_Primes[2] = 0x98badcfe;
	m_Primes[3] = 0x10325476;
	m_Primes[4] = 0xc3d2e1f0;
}

string SHA1Hasher::Digest() const
{
	stringstream ss;
	for (unsigned int m_Prime : m_Primes)
	{
		// Keep leading 0s
		ss << hex << setfill('0') << setw(8) << m_Prime;
	}

	return ss.str();
}

string SHA1Hasher::CalculateStringHash(const string& input)
{
	return string();
}
