#include <iostream>
#include <chrono>

#include "./Hashing/HashingAlgorithm.h"
#include "./Hashing/MD5Hash.h"
#include "./Hashing/SHA256Hash.h"

int main(int argc, char** argv)
{
    HashingAlgorithm* md5Hasher = new MD5Hasher();
    HashingAlgorithm* sha256Hasher = new SHA256Hasher();
    std::string input;

    while (true)
    {
        input = "";
        std::cin >> input;

        if (input == "exit")
        {
            break;
        }
        else if (input == "file")
        {
            /*
            * Tested files (Release mode), times include reading file and hashing,
            * so this may vary depending on harddrive (NVMe SSD for me). Tested only once:
            * Size (using filesystem::file_size / 1000) and duration (using chrono::high_resolution_clock) :
            * - 14847574 KB     => 134895 ms
            * - 5186093 KB      => 45846 ms     (27.9s were spent with File IO, and 17.9s with hashing)
            * - 1039630 KB      => 9304 ms      (5.7s were spent with File IO, and 3.6s with hashing)
            * - 431357 KB       => 3820 ms      (2.3s were spent with File IO, and 1.5s with hashing)
            * - 103972 KB       => 928 ms
            * - 1245 KB         => 11 ms
            * - 115 KB          => 1 ms
            */
            // Hash the file "test", can be changed at runtime
            size_t fileSize = 0;
            auto start = std::chrono::high_resolution_clock::now();
            std::string sha256Hash = sha256Hasher->CalculateFileHash(L"C:/Dev/Cpp/FileHasher/test");
            auto end = std::chrono::high_resolution_clock::now();
            std::cout << "File SHA256: " << sha256Hash << std::endl;
            std::cout << "Took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms, with size: " << fileSize / 1000 << " KB" << std::endl;

            start = std::chrono::high_resolution_clock::now();
            std::string md5Hash = md5Hasher->CalculateFileHash(L"C:/Dev/Cpp/FileHasher/test");
            end = std::chrono::high_resolution_clock::now();
            std::cout << "File MD5:    " << md5Hash << std::endl;
            std::cout << "Took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms, with size: " << fileSize / 1000 << " KB" << std::endl;
        }
        else
        {
            auto start = std::chrono::high_resolution_clock::now();
            std::string hash = sha256Hasher->CalculateStringHash(input);
            auto end = std::chrono::high_resolution_clock::now();
            std::cout << "SHA256: " << hash << std::endl;
            std::cout << "Took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds" << std::endl;
        }
    }

    delete md5Hasher;
    delete sha256Hasher;
}