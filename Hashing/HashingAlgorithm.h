#pragma once

class HashingAlgorithm
{
public:
	virtual char* CalculateHash(const wchar_t* filePath) = 0;
};