#include <cassert>
#include <sstream>
#include <iomanip>

#include "../Utility/FileUtility.h"
#include "../Utility/BitwiseUtility.h"
#include "./SHA512Hash.h"

using namespace std;

#define ROTR(n, c) BitUtil::rotr64(n, c)
#define XOR(a, b) (a ^ b)
#define RSH(a, b) (a >> b)

#define s0(w) (XOR(XOR(ROTR(w, 1), ROTR(w, 8)), RSH(w, 7)))
#define s1(w) (XOR(XOR(ROTR(w, 19), ROTR(w, 61)), RSH(w, 6)))

#define S1(e) (XOR(XOR(ROTR(e, 14), ROTR(e, 18)), ROTR(e, 41)))
#define S0(a) (XOR(XOR(ROTR(a, 28), ROTR(a, 34)), ROTR(a, 39)))
#define ch(e, f, g) (XOR((e & f), ((~e) & g)))
#define maj(a, b, c) (XOR(XOR((a & b), (a & c)), (b & c)))

constexpr size_t ENTRY_MESSAGE_SIZE = 80U;
constexpr size_t CHUNK_SIZE_BYTES = 128U;
constexpr size_t CHUNK_SIZE_BITS = 1024U;

std::string SHA512Hasher::Hash(const size_t fileSize)
{
	size_t paddingSize = 0U;
	uint8_t* padding = this->PreProcess(fileSize, paddingSize);
	if (!padding)
	{
		return "";
	}

	m_nBytesProcessed = 0U;
	if (!this->Process(padding, paddingSize))
	{
		delete[] padding;
		return "";
	}

	delete[] padding;

	string hash = this->Digest();
	this->ResetPrimes();

	return hash;
}

uint8_t* SHA512Hasher::PreProcess(const size_t fileSize, size_t& paddingSize)
{
	uint64_t L = fileSize * 8UL;
	uint64_t restBits = L % CHUNK_SIZE_BITS;
	uint64_t minExtraBitsNeeded = CHUNK_SIZE_BITS - restBits;
	// How many 0-bits we need for padding, should be at least 7
	uint64_t numKBits = (minExtraBitsNeeded - 1 - 128) % CHUNK_SIZE_BITS;

	// Pad with 1 '1' bit, k 0 bits, and the length as a 128-bit integer
	paddingSize = (1 + 128 + numKBits) / 8UL;
	uint8_t* buffer = new uint8_t[paddingSize];
	if (!buffer)
	{
		return nullptr;
	}

	// Fill out the padding
	// Set the first padded bit to 1, and also already the first 7 0-bits
	size_t padCounter = 0;
	buffer[padCounter++] = 0b10000000;
	numKBits -= 7;
	// Extra 8 bytes since for SHA512 L must be 128 bits, upper 64 bits will always be 0
	for (; padCounter <= numKBits / 8 + 8; padCounter++)
	{
		buffer[padCounter] = 0x00;
	}
	// append bit-length L creating a 128-bit big-endian integer
	for (int c = (sizeof(L) - 1) * 8U; c >= 0; c -= 8)
	{
		// "Iterate" through L's bytes by shifting the current byte completely to the right (least significant)
		// and discarding all higher (by casting to an unsigned byte)
		buffer[padCounter++] = static_cast<uint8_t>(L >> c);
	}

	// Our message together with the padding should now be a multiple of 128 bytes (1024 bits)
	assert((fileSize + paddingSize) % CHUNK_SIZE_BYTES == 0);

	return buffer;
}

bool SHA512Hasher::Process(const uint8_t* padding, const size_t paddingSize)
{
	while (m_pFileUtil->CanRead())
	{
		size_t currentBlockSize = 0U;
		uint8_t* buffer = GetDataBlock(paddingSize, padding, currentBlockSize);
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
				w[i] = BitUtil::AppendBytes<uint64_t>(buffer + chunk + i * 8);
			}

			// Extend the first 16 words into the remaining 64 words
			for (size_t i = 16; i < ENTRY_MESSAGE_SIZE; i++)
			{
				w[i] = w[i - 16] + s0(w[i - 15]) + w[i - 7] + s1(w[i - 2]);
			}

			uint64_t a = hPrime[0], b = hPrime[1], c = hPrime[2], d = hPrime[3], e = hPrime[4], f = hPrime[5], g = hPrime[6], h = hPrime[7];

			// Compression
			for (size_t i = 0; i < ENTRY_MESSAGE_SIZE; i++)
			{
				uint64_t temp1 = h + S1(e) + ch(e, f, g) + k[i] + w[i];
				uint64_t temp2 = S0(a) + maj(a, b, c);

				h = g; g = f; f = e; e = d + temp1; d = c; c = b; b = a; a = temp1 + temp2;
			}

			hPrime[0] += a; hPrime[1] += b; hPrime[2] += c; hPrime[3] += d;
			hPrime[4] += e; hPrime[5] += f; hPrime[6] += g; hPrime[7] += h;
		}

		delete[] buffer;
	}

	return true;
}

void SHA512Hasher::ResetPrimes()
{
	hPrime[0] = 0x6a09e667f3bcc908ULL;
	hPrime[1] = 0xbb67ae8584caa73bULL;
	hPrime[2] = 0x3c6ef372fe94f82bULL;
	hPrime[3] = 0xa54ff53a5f1d36f1ULL;
	hPrime[4] = 0x510e527fade682d1ULL;
	hPrime[5] = 0x9b05688c2b3e6c1fULL;
	hPrime[6] = 0x1f83d9abfb41bd6bULL;
	hPrime[7] = 0x5be0cd19137e2179ULL;
}

string SHA512Hasher::Digest()
{
	stringstream ss;
	for (size_t i = 0; i < 8; i++)
	{
		// Keep leading 0s
		ss << hex << setfill('0') << setw(16) << hPrime[i];
	}

	return ss.str();
}

string SHA512Hasher::CalculateStringHash(const string& input)
{
	return std::string();
}

SHA512Hasher::~SHA512Hasher()
{
}
