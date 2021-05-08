#include "pch.h"

#include "../../Hashing/HashingAlgorithm.h"
#include "../../Hashing/MD5Hash.h"
#include "../../Hashing/SHA1Hash.h"
#include "../../Hashing/SHA256Hash.h"
#include "../../Hashing/SHA512Hash.h"

// Set this to the directory where the test input files are,
// GoogleTestAdapter doesn't seem to support setting working directory
#define TEST_FILES_DIR L"C:/Dev/Cpp/FileHasher/Testing/TestFiles/"
#define EMPTY_TEST_FILE TEST_FILES_DIR L"empty.txt"
#define QUICKFOX_TEST_FILE TEST_FILES_DIR L"quickfox.txt"
#define RANDOMTEXT_TEST_FILE TEST_FILES_DIR L"random_text.txt"
#define RANDOMBYTES_TEST_FILE TEST_FILES_DIR L"random_bytes"
#define NONEXISTANT_TEST_FILE TEST_FILES_DIR L"IDONTEXIST.ZZZZZ"

/**
* ===================
* |	  MD5HashTest	|
* ===================
*/

TEST(MD5HashTest, HandleEmptyFile) 
{
	HashingAlgorithm* md5Hasher = new MD5Hasher;
	const std::string expectedHash = "d41d8cd98f00b204e9800998ecf8427e";
	const std::string actualHash = md5Hasher->CalculateFileHash(EMPTY_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete md5Hasher;
}
TEST(MD5HashTest, HandleBrownFox)
{
	HashingAlgorithm* md5Hasher = new MD5Hasher;
	const std::string expectedHash = "9e107d9d372bb6826bd81d3542a419d6";
	const std::string actualHash = md5Hasher->CalculateFileHash(QUICKFOX_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete md5Hasher;
}
TEST(MD5HashTest, HandleRandomText)
{
	HashingAlgorithm* md5Hasher = new MD5Hasher;
	const std::string expectedHash = "ef4b6ce35d28359b801810d7981a1bed";
	const std::string actualHash = md5Hasher->CalculateFileHash(RANDOMTEXT_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete md5Hasher;
}
TEST(MD5HashTest, HandleRandomBytes)
{
	HashingAlgorithm* md5Hasher = new MD5Hasher;
	const std::string expectedHash = "a8c9ab390419f00a90e45184a07d55c8";
	const std::string actualHash = md5Hasher->CalculateFileHash(RANDOMBYTES_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete md5Hasher;
}
TEST(MD5HashTest, HandleNonExistant)
{
	HashingAlgorithm* md5Hasher = new MD5Hasher;
	const std::string expectedHash = "";
	const std::string actualHash = md5Hasher->CalculateFileHash(NONEXISTANT_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete md5Hasher;
}

/**
* ===================
* |	  SHA1HashTest	|
* ===================
*/

TEST(SHA1HashTest, HandleEmptyFile)
{
	HashingAlgorithm* sha1Hasher = new SHA1Hasher;
	const std::string expectedHash = "da39a3ee5e6b4b0d3255bfef95601890afd80709";
	const std::string actualHash = sha1Hasher->CalculateFileHash(EMPTY_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete sha1Hasher;
}
TEST(SHA1HashTest, HandleBrownFox)
{
	HashingAlgorithm* sha1Hasher = new SHA1Hasher;
	const std::string expectedHash = "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12";
	const std::string actualHash = sha1Hasher->CalculateFileHash(QUICKFOX_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete sha1Hasher;
}
TEST(SHA1HashTest, HandleRandomText)
{
	HashingAlgorithm* sha1Hasher = new SHA1Hasher;
	const std::string expectedHash = "e6f65bf47f6e6295d96f6819d7b03cf1c0c99020";
	const std::string actualHash = sha1Hasher->CalculateFileHash(RANDOMTEXT_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete sha1Hasher;
}
TEST(SHA1HashTest, HandleRandomBytes)
{
	HashingAlgorithm* sha1Hasher = new SHA1Hasher;
	const std::string expectedHash = "978a9a9d686100c11b9198605594ce17434dcbb8";
	const std::string actualHash = sha1Hasher->CalculateFileHash(RANDOMBYTES_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete sha1Hasher;
}
TEST(SHA1HashTest, HandleNonExistant)
{
	HashingAlgorithm* sha1Hasher = new SHA1Hasher;
	const std::string expectedHash = "";
	const std::string actualHash = sha1Hasher->CalculateFileHash(NONEXISTANT_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete sha1Hasher;
}

/**
* ===================
* |	 SHA256HashTest	|
* ===================
*/

TEST(SHA256HashTest, HandleEmptyFile)
{
	HashingAlgorithm* sha256Hasher = new SHA256Hasher;
	const std::string expectedHash = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";
	const std::string actualHash = sha256Hasher->CalculateFileHash(EMPTY_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete sha256Hasher;
}
TEST(SHA256HashTest, HandleBrownFox)
{
	HashingAlgorithm* sha256Hasher = new SHA256Hasher;
	const std::string expectedHash = "d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592";
	const std::string actualHash = sha256Hasher->CalculateFileHash(QUICKFOX_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete sha256Hasher;
}
TEST(SHA256HashTest, HandleRandomText)
{
	HashingAlgorithm* sha256Hasher = new SHA256Hasher;
	const std::string expectedHash = "a9ad52174f5da3281a58ff7805ed73622512dbe8b4e31e8f6920992ef583919c";
	const std::string actualHash = sha256Hasher->CalculateFileHash(RANDOMTEXT_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete sha256Hasher;
}
TEST(SHA256HashTest, HandleRandomBytes)
{
	HashingAlgorithm* sha256Hasher = new SHA256Hasher;
	const std::string expectedHash = "c089c3f4fafda89a17edbb9d87cde4a5ff3ac0c3749d88441dc09e04f427410f";
	const std::string actualHash = sha256Hasher->CalculateFileHash(RANDOMBYTES_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete sha256Hasher;
}
TEST(SHA256HashTest, HandleNonExistant)
{
	HashingAlgorithm* sha256Hasher = new SHA256Hasher;
	const std::string expectedHash = "";
	const std::string actualHash = sha256Hasher->CalculateFileHash(NONEXISTANT_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete sha256Hasher;
}

/**
* ===================
* |	 SHA512HashTest	|
* ===================
*/

TEST(SHA512HashTest, HandleEmptyFile)
{
	HashingAlgorithm* sha512Hasher = new SHA512Hasher;
	const std::string expectedHash = "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e";
	const std::string actualHash = sha512Hasher->CalculateFileHash(EMPTY_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete sha512Hasher;
}
TEST(SHA512HashTest, HandleBrownFox)
{
	HashingAlgorithm* sha512Hasher = new SHA512Hasher;
	const std::string expectedHash = "07e547d9586f6a73f73fbac0435ed76951218fb7d0c8d788a309d785436bbb642e93a252a954f23912547d1e8a3b5ed6e1bfd7097821233fa0538f3db854fee6";
	const std::string actualHash = sha512Hasher->CalculateFileHash(QUICKFOX_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete sha512Hasher;
}
TEST(SHA512HashTest, HandleRandomText)
{
	HashingAlgorithm* sha512Hasher = new SHA512Hasher;
	const std::string expectedHash = "64fe2a7304ed70fd6fddebb2dd391bcea3a6bffa5b885484844db1bfcbad1bfcd71e7558ada0d851f302eae6a3f0b48bdb8a34e4154278242ea1c48e617552d8";
	const std::string actualHash = sha512Hasher->CalculateFileHash(RANDOMTEXT_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete sha512Hasher;
}
TEST(SHA512HashTest, HandleRandomBytes)
{
	HashingAlgorithm* sha512Hasher = new SHA512Hasher;
	const std::string expectedHash = "78e0c313f2ea2af003df177872d8a94553ceb1da000764bd4232294915df62da671fdd292c28b71e0287a1fda09d5e748704037ec6e9c2d6099a0ed06dc12b9e";
	const std::string actualHash = sha512Hasher->CalculateFileHash(RANDOMBYTES_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete sha512Hasher;
}
TEST(SHA512HashTest, HandleNonExistant)
{
	HashingAlgorithm* sha512Hasher = new SHA512Hasher;
	const std::string expectedHash = "";
	const std::string actualHash = sha512Hasher->CalculateFileHash(NONEXISTANT_TEST_FILE);
	EXPECT_EQ(actualHash, expectedHash);
	delete sha512Hasher;
}