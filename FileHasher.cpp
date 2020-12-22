#include <iostream>

#include "./Hashing/HashingAlgorithm.h"
#include "./Hashing/MD5Hash.h"
#include "./Hashing/SHA256Hash.h"

int main(int argc, char** argv)
{
    HashingAlgorithm* md5Hasher = new MD5Hasher();
    HashingAlgorithm* sha256Hasher = new SHA256Hasher();
    std::string md5Hash = md5Hasher->CalculateHash(L"../../wlPbDX12ex.jpg");
    std::string hash = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";
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
            // Hash the file "test", can be changed at runtime
            std::cout << "File SHA256: " << sha256Hasher->CalculateHash(L"C:/Dev/Cpp/FileHasher/test") << std::endl;
        }
        else
        {
            std::cout << "SHA256: " << sha256Hasher->CalculateHash(input) << std::endl;
        }
    }

    delete md5Hasher;
    delete sha256Hasher;
}