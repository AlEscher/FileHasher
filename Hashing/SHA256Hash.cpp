#include <cstddef>
#include <sstream>
#include <iomanip>

#include "../Utility/FileUtility.h"
#include "../Utility/BitwiseUtility.h"
#include "./SHA256Hash.h"

using namespace std;

#define ROTR(n, c) BitUtil::rotr32(n, c)
#define XOR(a, b) (a ^ b)
#define RSH(a, b) (a >> b)

#define s0(w) (XOR(XOR(ROTR(w, 7), ROTR(w, 18)), RSH(w, 3)))
#define s1(w) (XOR(XOR(ROTR(w, 17), ROTR(w, 19)), RSH(w, 10)))

#define S1(e) (XOR(XOR(ROTR(e, 6), ROTR(e, 11)), ROTR(e, 25)))
#define S0(a) (XOR(XOR(ROTR(a, 2), ROTR(a, 13)), ROTR(a, 22)))
#define ch(e, f, g) (XOR((e & f), ((~e) & g)))
#define maj(a, b, c) (XOR(XOR((a & b), (a & c)), (b & c)))

constexpr size_t ENTRY_MESSAGE_SIZE = 64U;

bool SHA256Hasher::Process(const uint8_t* padding, const size_t paddingSize)
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
			// 64-entry message schedule
			uint32_t w[ENTRY_MESSAGE_SIZE]{ 0 };

			// Load bytes of this chunk into first 16 32-bit words
			for (size_t i = 0; i < 16; i++)
			{
				// Append 4 bytes and put them into one variable
				w[i] = BitUtil::AppendBytes<uint32_t>(buffer + chunk + (i * 4));
			}

			// Extend the first 16 words into the remaining 48 words
			for (size_t i = 16; i < ENTRY_MESSAGE_SIZE; i++)
			{
				w[i] = w[i - 16] + s0(w[i - 15]) + w[i - 7] + s1(w[i - 2]);
			}

			uint32_t a = hPrime[0], b = hPrime[1], c = hPrime[2], d = hPrime[3], e = hPrime[4], f = hPrime[5], g = hPrime[6], h = hPrime[7];

			// Compression
			for (size_t i = 0; i < ENTRY_MESSAGE_SIZE; i++)
			{
				uint32_t temp1 = h + S1(e) + ch(e, f, g) + k[i] + w[i];
				uint32_t temp2 = S0(a) + maj(a, b, c);

				h = g; g = f; f = e; e = d + temp1; d = c; c = b; b = a; a = temp1 + temp2;
			}

			hPrime[0] += a; hPrime[1] += b; hPrime[2] += c; hPrime[3] += d;
			hPrime[4] += e; hPrime[5] += f; hPrime[6] += g; hPrime[7] += h;
		}

		delete[] buffer;
	} while (m_pFileUtil->CanRead());

	return true;
}

string SHA256Hasher::Digest() const
{
	stringstream ss;
	for (size_t i = 0; i < 8; i++) 
	{
		// Keep leading 0s
		ss << hex << setfill('0') << setw(8) << hPrime[i];
	}

	return ss.str();
}

void SHA256Hasher::ResetPrimes()
{
	hPrime[0] = 0x6a09e667U;
	hPrime[1] = 0xbb67ae85U;
	hPrime[2] = 0x3c6ef372U;
	hPrime[3] = 0xa54ff53aU;
	hPrime[4] = 0x510e527fU;
	hPrime[5] = 0x9b05688cU;
	hPrime[6] = 0x1f83d9abU;
	hPrime[7] = 0x5be0cd19U;
}

string SHA256Hasher::CalculateStringHash(const string& input)
{
	return "";
}

SHA256Hasher::~SHA256Hasher()
{

}
