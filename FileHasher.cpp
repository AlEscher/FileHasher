#include <iostream>

#include "./Hashing/HashingAlgorithm.h"
#include "./Hashing/MD5Hash.h"

int main(int argc, char** argv)
{
    HashingAlgorithm* md5Hasher = new MD5Hasher();
    char* md5Hash = md5Hasher->CalculateHash(L"../wlPbDX12ex.jpg");
    
    if (md5Hash != nullptr)
    {
        std::cout << md5Hash << std::endl;
    }

    delete[] md5Hash;
    delete md5Hasher;

    int end = getchar();
}