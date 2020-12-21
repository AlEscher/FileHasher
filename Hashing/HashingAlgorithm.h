#pragma once
#include <cstdint>
#include <vector>

class HashingAlgorithm
{
public:
	virtual char* CalculateHash(const wchar_t* filePath) = 0;
};