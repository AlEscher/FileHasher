#pragma once

#include "HashingAlgorithm.h"

class MD5Hasher : public HashingAlgorithm
{
public:
	std::string CalculateHash(const wchar_t* filePath);
	std::string CalculateHash(const std::string& input);
};