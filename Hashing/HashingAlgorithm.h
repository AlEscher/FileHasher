#pragma once
#include <cstdint>

class HashingAlgorithm
{
protected:
	// Reads the specified file into memory, and sets fileSize to the size of the file in bytes
	// Returned buffer will be null if anything fails
	char* ReadFileContent(const wchar_t* filePath, size_t& fileSize);
public:
	virtual char* CalculateHash(const wchar_t* filePath) = 0;
};