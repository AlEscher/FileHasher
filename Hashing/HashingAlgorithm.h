#pragma once
#include <cstdint>
#include <vector>
#include <string>

#include "../Utility/FileUtility.h"

class HashingAlgorithm
{
protected:
	FileUtil* fileUtil;
public:
	// Calculate file hash
	virtual std::string CalculateHash(const wchar_t* filePath) = 0;
	// Calculate file hash and set fileSize (not really needed, mostly for debugging purposes)
	virtual std::string CalculateHash(const wchar_t* filePath, size_t& fileSize) = 0;
	// Calculate hash for given string
	virtual std::string CalculateHash(const std::string& input) = 0;
	HashingAlgorithm()
	{
		this->fileUtil = new FileUtil;
	}
	virtual ~HashingAlgorithm()
	{
		delete this->fileUtil;
	}
};