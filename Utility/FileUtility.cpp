#include <filesystem>
#include <fstream>

#include "./FileUtility.h"

using namespace std;

FileUtil::FileUtil()
{

}

FileUtil::FileUtil(size_t blockSize)
{
	this->m_nBlockSize = blockSize;
}

FileUtil::~FileUtil()
{
	if (m_bIsOpen)
	{
		fclose(m_pInput);
	}
}

bool FileUtil::OpenFileStream(const char* filePath)
{
	m_bIsOpen = fopen_s(&m_pInput, filePath, "rb") == 0;
	return m_bIsOpen;
}

bool FileUtil::CanRead()
{
	return (m_bIsOpen && ferror(m_pInput) == 0 && feof(m_pInput) == 0);
}

uint8_t* FileUtil::GetNextBlock()
{
	if (!CanRead())
	{
		return nullptr;
	}

	uint8_t* block = new uint8_t[m_nBlockSize];
	if (!block)
	{
		return nullptr;
	}

	size_t elementsRead = fread(block, 1U, m_nBlockSize, m_pInput);
	if (elementsRead != m_nBlockSize && feof(m_pInput) == 0)
	{
		// If we read less than one blocksize and End Of File is not reached, something went wrong
		delete[] block;
		return nullptr;
	}

	return block;
}

size_t FileUtil::BytesRemaining()
{
	if (!CanRead())
	{
		return 0U;
	}

	long bytesRead = ftell(m_pInput);
	if (bytesRead == -1L)
	{
		return 0U;
	}

	return m_nTotalFileSIze - bytesRead;
}

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
	input.close();

	fileSize = size;
	return bufferVec;
}