#pragma once
#include <vector>

namespace FileUtil
{
	// Get the size of a file on disk.
	// Returns file size on success, 0 otherwise
	size_t GetFileSize(const wchar_t* filePath);
	// Reads the specified file into memory, and sets fileSize to the size of the file in bytes
	// fileSize will be set to 0 if anything fails
	std::vector<uint8_t> ReadFileContent(const wchar_t* filePath, size_t& fileSize);
}