#include <cstddef>

#include "./SHA256Hash.h"

char* SHA256Hasher::CalculateHash(const wchar_t* filePath)
{
	std::size_t fileSize = 0;
	char* buffer = ReadFileContent(filePath, fileSize);
	if (!buffer)
	{
		return nullptr;
	}

	delete[] buffer;
	return nullptr;
}