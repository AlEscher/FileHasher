#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>

#include "./FileUtility.h"

#define GCC_COMPILER (defined(__GNUC__) && !defined(__clang__))
#if GCC_COMPILER
#include <string.h>
#include <stdio.h>
#endif

using namespace std;

FileUtil::FileUtil()
= default;

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

bool FileUtil::OpenFileStreamW(const wchar_t* filePath)
{
	Reset();
#ifdef GCC_COMPILER
	auto strLen = wcslen(filePath) + 1;
	auto mbFilePath = std::make_unique<char[]>(strLen);
	wcstombs(mbFilePath.get(), filePath, strLen);
	m_pInput = fopen(mbFilePath.get(), "rb");
	m_bIsOpen = m_pInput != nullptr;
#else
	m_bIsOpen = _wfopen_s(&m_pInput, filePath, L"rb") == 0;
#endif
	if (m_bIsOpen)
	{
		m_nTotalFileSize = FileUtil::GetFileSizeW(filePath).value_or(0U);
	}

	return m_bIsOpen;
}

bool FileUtil::CanRead() const
{
	return IsOpen() && (BytesRemaining() > 0);
}

bool FileUtil::IsOpen() const
{
	return m_bIsOpen && (ferror(m_pInput) == 0);
}

std::unique_ptr<uint8_t[]> FileUtil::GetNextBlock()
{
	if (!CanRead())
	{
		return nullptr;
	}

	const size_t size = min(m_nBlockSize, BytesRemaining());
	auto block = make_unique<uint8_t[]>(size);
	if (!block)
	{
		return nullptr;
	}

	if (const size_t elementsRead = fread(block.get(), 1U, size, m_pInput); elementsRead != size && BytesRemaining() > 0)
	{
		// If we read less than we intended to and End Of File is not reached, something went wrong
		return nullptr;
	}

	return block;
}

size_t FileUtil::BytesRemaining() const
{
	if (!m_bIsOpen)
	{
		return 0U;
	}

#ifdef GCC_COMPILER
	// Will not work for large files, but GCC seems to have no proper method for this
	const int64_t bytesRead = ftell(m_pInput);
#else
	const int64_t bytesRead = _ftelli64(m_pInput);
#endif
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

optional<size_t> FileUtil::GetFileSizeW(const wchar_t* filePath)
{
	return optional<size_t>{filesystem::file_size(filePath)};
}

optional<size_t> FileUtil::GetFileSizeA(const char* filePath)
{
#if GCC_COMPILER
	const size_t len = strlen(filePath) + 1
#else
	const size_t len = strnlen_s(filePath, 1024U) + 1;
#endif
	;
	size_t ret = 0U;
	auto* widePath = new wchar_t[len];

	mbstowcs(widePath, filePath, len);
	const optional<size_t> fileSize = FileUtil::GetFileSizeW(widePath);
	delete[] widePath;

	return fileSize;
}

vector<uint8_t> FileUtil::ReadFileContent(const wchar_t* filePath, size_t& fileSize)
{
	vector<uint8_t> bufferVec;
	fileSize = 0;

	optional<size_t> size = FileUtil::GetFileSizeW(filePath);
	if (!size.has_value())
	{
		return bufferVec;
	}

#ifdef GCC_COMPILER
	auto strLen = wcslen(filePath) + 1;
	auto mbFilePath = std::make_unique<char[]>(strLen);
	wcstombs(mbFilePath.get(), filePath, strLen);
	ifstream input{ mbFilePath.get(), ios::binary };
#else
	ifstream input{ filePath, ios::binary };
#endif
	if (input.fail())
	{
		return bufferVec;
	}

	bufferVec = vector<uint8_t>((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
	input.close();

	fileSize = size.value();
	return bufferVec;
}