#include <iostream>

#include "./Hashing/HashingAlgorithm.h"
#include "./Hashing/MD5Hash.h"
#include "./Hashing/SHA256Hash.h"

int main(int argc, char** argv)
{
    HashingAlgorithm* md5Hasher = new MD5Hasher();
    HashingAlgorithm* sha256Hasher = new SHA256Hasher();
    char* md5Hash = md5Hasher->CalculateHash(L"../../wlPbDX12ex.jpg");
    char* sha256Hash = sha256Hasher->CalculateHash(L"");
    
    if (md5Hash != nullptr)
    {
        std::cout << md5Hash << std::endl;
    }

    delete[] md5Hash;
    delete md5Hasher;
    delete[] sha256Hash;
    delete sha256Hasher;

    int end = getchar();
}