#include <cstddef>
#include <sstream>
#include <iomanip>

#include "../Utility/FileUtility.h"
#include "../Utility/BitwiseUtility.h"
#include "./SHA256Hash.h"

using namespace std;

#define ROTR(n, c) BitUtil::rotr32(n, c)
#define XOR(a, b) ((a) ^ (b))
#define RSH(a, b) ((a) >> (b))

#define s0(w) (XOR(XOR(ROTR(w, 7), ROTR(w, 18)), RSH(w, 3)))
#define s1(w) (XOR(XOR(ROTR(w, 17), ROTR(w, 19)), RSH(w, 10)))

#define S1(e) (XOR(XOR(ROTR(e, 6), ROTR(e, 11)), ROTR(e, 25)))
#define S0(a) (XOR(XOR(ROTR(a, 2), ROTR(a, 13)), ROTR(a, 22)))
#define ch(e, f, g) (XOR(((e) & (f)), ((~(e)) & (g))))
#define maj(a, b, c) (XOR(XOR(((a) & (b)), ((a) & (c))), ((b) & (c))))

constexpr size_t ENTRY_MESSAGE_SIZE = 64U;

bool SHA256Hasher::Process(const uint8_t* padding, const size_t paddingSize)
{
	do
	{
		size_t currentBlockSize = 0U;
		auto buffer = GetDataBlock(paddingSize, padding, currentBlockSize);
		if (!buffer)
		{
			return false;
		}

		// Break one block of the message into 512-bit chunks
		for (size_t chunk = 0; chunk < currentBlockSize; chunk += CHUNK_SIZE_BYTES)
		{
			// 64-entry message schedule
			uint32_t w[ENTRY_MESSAGE_SIZE]{ 0 };

			// Load bytes of this chunk into first 16 32-bit words
			for (size_t i = 0; i < 16; i++)
			{
				// Append 4 bytes and put them into one variable
				w[i] = BitUtil::AppendBytes<uint32_t>(buffer.get() + chunk + (i * 4));
			}

			// Extend the first 16 words into the remaining 48 words
			for (size_t i = 16; i < ENTRY_MESSAGE_SIZE; i++)
			{
				w[i] = w[i - 16] + s0(w[i - 15]) + w[i - 7] + s1(w[i - 2]);
			}

			uint32_t a = m_hPrime[0], b = m_hPrime[1], c = m_hPrime[2], d = m_hPrime[3], e = m_hPrime[4], f = m_hPrime[5], g = m_hPrime[6], h = m_hPrime[7];

			// Compression
			for (size_t i = 0; i < ENTRY_MESSAGE_SIZE; i++)
			{
				const uint32_t temp1 = h + S1(e) + ch(e, f, g) + m_k[i] + w[i];
				const uint32_t temp2 = S0(a) + maj(a, b, c);

				h = g; g = f; f = e; e = d + temp1; d = c; c = b; b = a; a = temp1 + temp2;
			}

			m_hPrime[0] += a; m_hPrime[1] += b; m_hPrime[2] += c; m_hPrime[3] += d;
			m_hPrime[4] += e; m_hPrime[5] += f; m_hPrime[6] += g; m_hPrime[7] += h;
		}
		
	} while (m_pFileUtil->CanRead() && !m_bStop);

	return true;
}

string SHA256Hasher::Digest() const
{
	stringstream ss;
	for (unsigned int i : m_hPrime)
	{
		// Keep leading 0s
		ss << hex << setfill('0') << setw(8) << i;
	}

	return ss.str();
}

void SHA256Hasher::ResetPrimes()
{
	m_hPrime[0] = 0x6a09e667U;
	m_hPrime[1] = 0xbb67ae85U;
	m_hPrime[2] = 0x3c6ef372U;
	m_hPrime[3] = 0xa54ff53aU;
	m_hPrime[4] = 0x510e527fU;
	m_hPrime[5] = 0x9b05688cU;
	m_hPrime[6] = 0x1f83d9abU;
	m_hPrime[7] = 0x5be0cd19U;
}

string SHA256Hasher::CalculateStringHash(const string& input)
{
	return "";
}

SHA256Hasher::~SHA256Hasher()
= default;
