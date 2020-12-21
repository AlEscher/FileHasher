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
    std::string sha256Hash = sha256Hasher->CalculateHash("");
    
    std::cout << md5Hash << std::endl;
    std::cout << sha256Hash << std::endl;

    if (sha256Hash != hash)
    {
        std::cout << "SHA256 Hash changed!" << std::endl;
    }

    delete md5Hasher;
    delete sha256Hasher;

    int end = getchar();
}