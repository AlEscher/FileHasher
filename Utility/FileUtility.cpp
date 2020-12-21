#include <filesystem>
#include <fstream>

#include "./FileUtility.h"

using namespace std;

size_t FileUtil::GetFileSize(const wchar_t* filePath)
{
	size_t size = 0;
	try
	{
		size = filesystem::file_size(filePath);
	}
	catch (filesystem::filesystem_error& e)
	{
		const char* error = e.what();
	}

	return size;
}

vector<uint8_t> FileUtil::ReadFileContent(const wchar_t* filePath, size_t& fileSize)
{
	vector<uint8_t> bufferVec;
	fileSize = 0;

	uintmax_t size = FileUtil::GetFileSize(filePath);
	if (size == 0)
	{
		return bufferVec;
	}

	ifstream input(filePath, ios::binary);
	if (input.fail())
	{
		return bufferVec;
	}

	bufferVec = vector<uint8_t>((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());

	fileSize = size;
	return bufferVec;
}