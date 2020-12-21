#include <filesystem>
#include <cstdint>
#include <fstream>
#include <vector>

#include "./HashingAlgorithm.h"

char* HashingAlgorithm::ReadFileContent(const wchar_t* filePath, size_t& fileSize)
{
	std::error_code errorCode;
	std::uintmax_t size = 0;
	try 
	{
		size = std::filesystem::file_size(filePath);
	}
	catch (std::filesystem::filesystem_error& e)
	{
		const char* error = e.what();
		int i = 0;
	}

	if (errorCode.value() != 0)
	{
		return nullptr;
	}

	fileSize = size;

	char* buffer = new char[size];
	if (!buffer)
	{
		return nullptr;
	}

	std::ifstream input(filePath, std::ios::binary);
	if (input.fail())
	{
		return nullptr;
	}
	
	if (!input.read(buffer, size))
	{
		return nullptr;
	}

	return buffer;
}