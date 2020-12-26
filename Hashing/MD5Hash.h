#pragma once

#include "HashingAlgorithm.h"

class MD5Hasher : public HashingAlgorithm
{
public:
	std::string CalculateFileHash(const wchar_t* filePath);
	std::string CalculateFileHash(const char* filePath);
	std::string CalculateFileHash(const wchar_t* filePath, size_t& fileSize);
	std::string CalculateStringHash(const std::string& input);
};