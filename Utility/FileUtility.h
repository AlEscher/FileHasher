#pragma once
#include <vector>

namespace FileUtil
{
	// Reads the specified file into memory, and sets fileSize to the size of the file in bytes
	// fileSize will be set to 0 if anything fails
	size_t GetFileSize(const wchar_t* filePath);
	std::vector<uint8_t> ReadFileContent(const wchar_t* filePath, size_t& fileSize);
}