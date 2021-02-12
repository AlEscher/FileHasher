#include <cassert>

#include "MD5Hash.h"
#include "../Utility/FileUtility.h"

using namespace std;

bool MD5Hasher::Process(const uint8_t* padding, const size_t paddingSize)
{
	return false;
}

void MD5Hasher::ResetPrimes()
{
}

std::string MD5Hasher::Digest() const
{
	return std::string();
}

std::string MD5Hasher::CalculateStringHash(const std::string& input)
{
	return "";
}