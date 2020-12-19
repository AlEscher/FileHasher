#pragma once

#include "HashingAlgorithm.h"

class MD5Hasher : public HashingAlgorithm
{
public:
	char* CalculateHash(const wchar_t* filePath);
};