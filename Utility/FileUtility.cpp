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

bool FileUtil::OpenFileStreamA(const char* filePath)
{
	Reset();
	m_bIsOpen = fopen_s(&m_pInput, filePath, "rb") == 0;
	if (m_bIsOpen)
	{
		m_nTotalFileSize = FileUtil::GetFileSizeA(filePath);
	}

	return m_bIsOpen;
}

bool FileUtil::OpenFileStreamW(const wchar_t* filePath)
{
	size_t len = wcsnlen_s(filePath, 1024U);
	size_t ret = 0U;
	bool success = false;
	char* path = new char[len + 1];

	wcstombs_s(&ret, path, len + 1, filePath, len);
	success = FileUtil::OpenFileStreamA(path);
	delete[] path;

	return success;
}

bool FileUtil::CanRead()
{
	return (m_bIsOpen && (ferror(m_pInput) == 0) && (BytesRemaining() > 0));
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
	if (elementsRead != m_nBlockSize && BytesRemaining() > 0)
	{
		// If we read less than one blocksize and End Of File is not reached, something went wrong
		delete[] block;
		return nullptr;
	}

	return block;
}

size_t FileUtil::BytesRemaining()
{
	if (!m_bIsOpen)
	{
		return 0U;
	}

	int64_t bytesRead = _ftelli64(m_pInput);
	if (bytesRead == -1LL)
	{
		return 0U;
	}

	return m_nTotalFileSize - bytesRead;
}

void FileUtil::Reset()
{
	if (m_bIsOpen)
	{
		fclose(m_pInput);
		m_nTotalFileSize = 0U;
		m_bIsOpen = false;
	}
}

size_t FileUtil::GetFileSizeW(const wchar_t* filePath)
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

size_t FileUtil::GetFileSizeA(const char* filePath)
{
	size_t len = strnlen_s(filePath, 1024U) + 1;
	size_t fileSize = 0U;
	size_t ret = 0U;
	wchar_t* widePath = new wchar_t[len];

	mbstowcs_s(&ret, widePath, len, filePath, len - 1);
	fileSize = FileUtil::GetFileSizeW(widePath);
	delete[] widePath;

	return fileSize;
}

vector<uint8_t> FileUtil::ReadFileContent(const wchar_t* filePath, size_t& fileSize)
{
	vector<uint8_t> bufferVec;
	fileSize = 0;

	uintmax_t size = FileUtil::GetFileSizeW(filePath);
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