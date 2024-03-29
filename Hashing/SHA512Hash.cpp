#include <sstream>
#include <iomanip>

#include "../Utility/FileUtility.h"
#include "../Utility/BitwiseUtility.h"
#include "./SHA512Hash.h"

using namespace std;

#define ROTR(n, c) BitUtil::rotr64(n, c)
#define XOR(a, b) ((a) ^ (b))
#define RSH(a, b) ((a) >> (b))

#define s0(w) (XOR(XOR(ROTR(w, 1), ROTR(w, 8)), RSH(w, 7)))
#define s1(w) (XOR(XOR(ROTR(w, 19), ROTR(w, 61)), RSH(w, 6)))

#define S1(e) (XOR(XOR(ROTR(e, 14), ROTR(e, 18)), ROTR(e, 41)))
#define S0(a) (XOR(XOR(ROTR(a, 28), ROTR(a, 34)), ROTR(a, 39)))
#define ch(e, f, g) (XOR(((e) & (f)), ((~(e)) & (g))))
#define maj(a, b, c) (XOR(XOR(((a) & (b)), ((a) & (c))), ((b) & (c))))

constexpr size_t ENTRY_MESSAGE_SIZE = 80U;


bool SHA512Hasher::Process(unique_ptr<uint8_t[]> padding, size_t paddingSize)
{
	do
	{
		size_t currentBlockSize = 0U;
		auto buffer = GetDataBlock(paddingSize, padding, currentBlockSize);
		if (!buffer)
		{
			return false;
		}

		// Break one block of the message into 1024-bit chunks
		for (size_t chunk = 0; chunk < currentBlockSize; chunk += CHUNK_SIZE_BYTES)
		{
			// 80-entry message schedule
			uint64_t w[ENTRY_MESSAGE_SIZE]{ 0 };

			// Load bytes of this chunk into first 16 64-bit words
			for (size_t i = 0; i < 16; i++)
			{
				// Combine 8 bytes into one 64-bit entry
				w[i] = BitUtil::AppendBytes<uint64_t>(buffer.get() + chunk + i * 8);
			}

			// Extend the first 16 words into the remaining 64 words
			for (size_t i = 16; i < ENTRY_MESSAGE_SIZE; i++)
			{
				w[i] = w[i - 16] + s0(w[i - 15]) + w[i - 7] + s1(w[i - 2]);
			}

			uint64_t a = m_primes[0], b = m_primes[1], c = m_primes[2], d = m_primes[3], e = m_primes[4], f = m_primes[5], g = m_primes[6], h = m_primes[7];

			// Compression
			for (size_t i = 0; i < ENTRY_MESSAGE_SIZE; i++)
			{
				const uint64_t temp1 = h + S1(e) + ch(e, f, g) + m_constants[i] + w[i];
				const uint64_t temp2 = S0(a) + maj(a, b, c);

				h = g; g = f; f = e; e = d + temp1; d = c; c = b; b = a; a = temp1 + temp2;
			}

			m_primes[0] += a; m_primes[1] += b; m_primes[2] += c; m_primes[3] += d;
			m_primes[4] += e; m_primes[5] += f; m_primes[6] += g; m_primes[7] += h;
		}

	} while (m_pFileUtil->CanRead() && !m_bStop);

	return true;
}

void SHA512Hasher::ResetPrimes()
{
	m_primes[0] = 0x6a09e667f3bcc908ULL;
	m_primes[1] = 0xbb67ae8584caa73bULL;
	m_primes[2] = 0x3c6ef372fe94f82bULL;
	m_primes[3] = 0xa54ff53a5f1d36f1ULL;
	m_primes[4] = 0x510e527fade682d1ULL;
	m_primes[5] = 0x9b05688c2b3e6c1fULL;
	m_primes[6] = 0x1f83d9abfb41bd6bULL;
	m_primes[7] = 0x5be0cd19137e2179ULL;
}

string SHA512Hasher::Digest() const
{
	stringstream ss;
	for (unsigned long long m_Prime : m_primes)
	{
		// Keep leading 0s
		ss << hex << setfill('0') << setw(16) << m_Prime;
	}

	return ss.str();
}

string SHA512Hasher::CalculateStringHash(const string& input)
{
	return std::string();
}

SHA512Hasher::~SHA512Hasher()
= default;
