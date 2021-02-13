#include <iomanip>
#include <sstream>

#include "MD5Hash.h"
#include "../Utility/FileUtility.h"
#include "../Utility/BitwiseUtility.h"

using namespace std;

#define LEFTROT(n, s) ((n << s) | (n >> (32 - s)))

constexpr size_t ENTRY_MESSAGE_SIZE = 16U;

bool MD5Hasher::Process(const uint8_t* padding, const size_t paddingSize)
{
	while (m_pFileUtil->CanRead())
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
				M[i] = BitUtil::SwapEndian32(BitUtil::AppendBytes<uint32_t>(buffer + chunk + (i * 4)));
			}

			uint32_t A = m_Primes[0], B = m_Primes[1], C = m_Primes[2], D = m_Primes[3];

			for (size_t i = 0; i < 64; i++)
			{
				uint32_t F = 0U;
				size_t g = 0U;

				if (0 <= i && i < 16)
				{
					F = (B & C) | (~B & D);
					g = i;
				}
				else if (16 <= i && i < 32)
				{
					F = (D & B) | (~D & C);
					g = (5 * i + 1) % 16;
				}
				else if (32 <= i && i < 48)
				{
					F = B ^ C ^ D;
					g = (3 * i + 5) % 16;
				}
				else if (48 <= i && i < 64)
				{
					F = C ^ (B | ~D);
					g = (7 * i) % 16;
				}

				F += A + m_Constants[i] + M[g];
				A = D; D = C; C = B;
				B += LEFTROT(F, m_Shifts[i]);
			}

			m_Primes[0] += A; m_Primes[1] += B; m_Primes[2] += C; m_Primes[3] += D;
		}

		delete[] buffer;
	}

	return true;
}

void MD5Hasher::ResetPrimes()
{
	m_Primes[0] = 0x67452301;
	m_Primes[1] = 0xefcdab89;
	m_Primes[2] = 0x98badcfe;
	m_Primes[3] = 0x10325476;
}

std::string MD5Hasher::Digest() const
{
	stringstream ss;
	for (size_t i = 0; i < 4; i++)
	{
		// Keep leading 0s
		ss << hex << setfill('0') << setw(8) << BitUtil::SwapEndian32(m_Primes[i]);
	}

	return ss.str();
}

std::string MD5Hasher::CalculateStringHash(const std::string& input)
{
	return "";
}