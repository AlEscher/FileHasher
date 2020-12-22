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
    
    //std::cout << md5Hash << std::endl;
    std::cout << sha256Hasher->CalculateHash("") << std::endl;
    std::cout << sha256Hasher->CalculateHash("hallo") << std::endl;
    // File containing the word "hallo"
    std::cout << sha256Hasher->CalculateHash(L"C:/Dev/Cpp/FileHasher/test.txt") << std::endl;
    std::cout << sha256Hasher->CalculateHash(L"C:/Dev/Cpp/FileHasher/wlPbDX12ex.jpg") << std::endl;

    delete md5Hasher;
    delete sha256Hasher;

    int end = getchar();
}