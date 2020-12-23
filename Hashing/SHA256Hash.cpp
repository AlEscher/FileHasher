#include <cstddef>
#include <cassert>
#include <sstream>
#include <iomanip>

#include "../Utility/FileUtility.h"
#include "../Utility/BitwiseUtility.h"
#include "./SHA256Hash.h"

using namespace std;
using namespace BitUtil;

#define ROTR(n, c) rotr32(n, c)
#define XOR(a, b) (a ^ b)
#define RSH(a, b) (a >> b)

#define s0(w) (XOR(XOR(ROTR(w, 7), ROTR(w, 18)), RSH(w, 3)))
#define s1(w) (XOR(XOR(ROTR(w, 17), ROTR(w, 19)), RSH(w, 10)))

#define S1(e) (XOR(XOR(ROTR(e, 6), ROTR(e, 11)), ROTR(e, 25)))
#define S0(a) (XOR(XOR(ROTR(a, 2), ROTR(a, 13)), ROTR(a, 22)))
#define ch(e, f, g) (XOR((e & f), ((~e) & g)))
#define maj(a, b, c) (XOR(XOR((a & b), (a & c)), (b & c)))

bool SHA256Hasher::PreProcess(vector<uint8_t>& buffer)
{
	size_t oldByteSize = buffer.size();
	uint64_t L = oldByteSize * 8UL;
	uint64_t restBits = L % 512;
	uint64_t minExtraBitsNeeded = 512 - restBits;
	// How many 0-bits we need for padding, should be at least 7
	uint64_t numKBits = (minExtraBitsNeeded - 1 - 64) % 512;

	// Pad with 1 '1' bit, k 0 bits, and the length as a 64-bit integer
	try
	{
		buffer.resize(oldByteSize + (1 + 64 + numKBits) / 8UL);
	}
	catch (const std::bad_alloc&)
	{
		return false;
	}

	// Fill out the padding
	// Set the first padded bit to 1, and also already the first 7 0-bits
	size_t padCounter = 0;
	buffer[oldByteSize + padCounter++] = 0b10000000;
	numKBits -= 7;
	for (; padCounter <= numKBits / 8; padCounter++)
	{
		buffer[oldByteSize + padCounter] = 0x00;
	}
	// append bit-length L as 64-bit big-endian integer
	for (int c = (sizeof(L) - 1) * 8U; c >= 0; c-=8)
	{
		// "Iterate" through L's bytes by shifting the current byte completely to the right (least significant)
		// and discarding all higher (by casting to an unsigned byte)
		buffer[oldByteSize + padCounter++] = static_cast<uint8_t>(L >> c);
	}

	// Our message should now be a multiple of 64 bytes (512 bits)
	assert(buffer.size() % 64 == 0);

	return true;
}

bool SHA256Hasher::Process(vector<uint8_t>& buffer)
{
	// Break message into 512-bit chunks
	for (size_t chunk = 0; chunk < buffer.size(); chunk += 64)
	{
		// 64-entry message schedule
		uint32_t w[64]{ 0 };

		// Load bytes of this chunk into first 16 32-bit words
		for (size_t i = 0; i < 16; i++)
		{
			// Take first byte and shift into upper (first) byte of a 32-bit word
			uint32_t first = buffer[chunk + i*4 + 0] << 24;
			uint32_t second = buffer[chunk + i*4 + 1] << 16;
			uint32_t third = buffer[chunk + i*4 + 2] << 8;
			uint32_t fourth = buffer[chunk + i*4 + 3];
			w[i] = first | second | third | fourth;
		}

		// Extend the first 16 words into the remaining 48 words
		for (size_t i = 16; i < 64; i++)
		{
			w[i] = w[i - 16] + s0(w[i - 15]) + w[i - 7] + s1(w[i - 2]);
		}

		uint32_t a = hPrime[0], b = hPrime[1], c = hPrime[2], d = hPrime[3], e = hPrime[4], f = hPrime[5], g = hPrime[6], h = hPrime[7];

		// Compression
		for (size_t i = 0; i < 64; i++)
		{
			uint32_t temp1 = h + S1(e) + ch(e, f, g) + k[i] + w[i];
			uint32_t temp2 = S0(a) + maj(a, b, c);

			h = g; g = f; f = e; e = d + temp1; d = c; c = b; b = a; a = temp1 + temp2;
		}

		hPrime[0] += a; hPrime[1] += b; hPrime[2] += c; hPrime[3] += d;
		hPrime[4] += e; hPrime[5] += f; hPrime[6] += g; hPrime[7] += h;
	}

	return true;
}

string SHA256Hasher::Digest()
{
	stringstream ss;
	for (size_t i = 0; i < 8; i++) 
	{
		// Keep leading 0s
		ss << hex << setfill('0') << setw(8) << hPrime[i];
	}

	return ss.str();
}

void SHA256Hasher::Reset()
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

string SHA256Hasher::Hash(vector<uint8_t>& buffer)
{
	// Message needs to have a length that is a multiple of 512 bits
	if (!this->PreProcess(buffer))
	{
		return "";
	}

	if (!this->Process(buffer))
	{
		return "";
	}

	string hash = this->Digest();
	this->Reset();

	return hash;
}

string SHA256Hasher::CalculateHash(const wchar_t* filePath)
{
	size_t fileSize = 0;
	vector<uint8_t> buffer = FileUtil::ReadFileContent(filePath, fileSize);
	if (fileSize == 0)
	{
		return "";
	}

	return this->Hash(buffer);
}

string SHA256Hasher::CalculateHash(const wchar_t* filePath, size_t& fileSize)
{
	size_t fileSizeLoc = fileSize = 0;
	vector<uint8_t> buffer = FileUtil::ReadFileContent(filePath, fileSizeLoc);
	if (fileSizeLoc == 0)
	{
		return "";
	}
	fileSize = fileSizeLoc;
	return this->Hash(buffer);
}

string SHA256Hasher::CalculateHash(const string& input)
{
	vector<uint8_t> buffer;
	stringstream ss;
	ss << input;
	buffer = vector<uint8_t>((istreambuf_iterator<char>(ss)), istreambuf_iterator<char>());

	return this->Hash(buffer);
}
