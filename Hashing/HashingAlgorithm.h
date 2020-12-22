#pragma once
#include <cstdint>
#include <vector>
#include <string>

class HashingAlgorithm
{
public:
	// Calculate file hash
	virtual std::string CalculateHash(const wchar_t* filePath) = 0;
	// Calculate hash for given string
	virtual std::string CalculateHash(const std::string& input) = 0;
};