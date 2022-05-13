#include <chrono>
#include <iostream>
#include <sstream>
#include <array>

#include "./Hashing/HashingAlgorithm.h"
#include "./Hashing/MD5Hash.h"
#include "./Hashing/SHA1Hash.h"
#include "./Hashing/SHA256Hash.h"
#include "./Hashing/SHA512Hash.h"

using namespace std;

constexpr size_t NUM_ITERATIONS = 4000;

const wstring emptyFilePath = L"C:/Dev/cppProfilerBenchmark/CppProfilerBenchmark/FileHasher/Testing/TestFiles/empty.txt";
const wstring quickFoxFilePath = L"C:/Dev/cppProfilerBenchmark/CppProfilerBenchmark/FileHasher/Testing/TestFiles/quickfox.txt";
const wstring randomTextFilePath = L"C:/Dev/cppProfilerBenchmark/CppProfilerBenchmark/FileHasher/Testing/TestFiles/random_text.txt";
const wstring randomBytesFilePath = L"C:/Dev/cppProfilerBenchmark/CppProfilerBenchmark/FileHasher/Testing/TestFiles/random_bytes";
const array<wstring, 4> files{emptyFilePath, quickFoxFilePath, randomTextFilePath, randomBytesFilePath};

int main()
{
    unique_ptr<HashingAlgorithm> sha256Hasher = make_unique<SHA256Hasher>();
    unique_ptr<HashingAlgorithm> sha512Hasher = make_unique<SHA512Hasher>();
    unique_ptr<HashingAlgorithm> md5Hasher = make_unique<MD5Hasher>();
    unique_ptr<HashingAlgorithm> sha1Hasher = make_unique<SHA1Hasher>();

    for (size_t i = 0; i < NUM_ITERATIONS; i++)
    {
        for (const auto& file : files)
        {
            const auto sha256 = sha256Hasher->CalculateFileHash(file.c_str());
            const auto sha512 = sha512Hasher->CalculateFileHash(file.c_str());
            const auto shaMD5 = md5Hasher->CalculateFileHash(file.c_str());
            const auto sha1 = sha1Hasher->CalculateFileHash(file.c_str());

            sha256Hasher->Reset();
            sha512Hasher->Reset();
            md5Hasher->Reset();
            sha1Hasher->Reset();

            if (sha256.empty() || sha512.empty() || shaMD5.empty() || sha1.empty())
            {
                wcout << L"Failure at run " << i << L" for file " << file << endl;
                return -1;
            }
        }
    }

    cout << "Success" << endl;
    return 0;
}