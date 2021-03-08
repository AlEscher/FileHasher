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

bool FileUtil::OpenFileStreamW(const wchar_t* filePath)
{
	Reset();
	m_bIsOpen = _wfopen_s(&m_pInput, filePath, L"rb") == 0;
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

uint8_t* FileUtil::GetNextBlock()
{
	if (!CanRead())
	{
		return nullptr;
	}

	size_t size = std::min(m_nBlockSize, BytesRemaining());
	uint8_t* block = new uint8_t[size];
	if (!block)
	{
		return nullptr;
	}

	size_t elementsRead = fread(block, 1U, size, m_pInput);
	if (elementsRead != size && BytesRemaining() > 0)
	{
		// If we read less than we intended to and End Of File is not reached, something went wrong
		delete[] block;
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

optional<size_t> FileUtil::GetFileSizeW(const wchar_t* filePath)
{
	try
	{
		return optional<size_t>{filesystem::file_size(filePath)};
	}
	catch (filesystem::filesystem_error)
	{
		return nullopt;
	}
}

optional<size_t> FileUtil::GetFileSizeA(const char* filePath)
{
	size_t len = strnlen_s(filePath, 1024U) + 1;
	size_t ret = 0U;
	wchar_t* widePath = new wchar_t[len];

	mbstowcs_s(&ret, widePath, len, filePath, len - 1);
	optional<size_t> fileSize = FileUtil::GetFileSizeW(widePath);
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

	ifstream input(filePath, ios::binary);
	if (input.fail())
	{
		return bufferVec;
	}

	bufferVec = vector<uint8_t>((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
	input.close();

	fileSize = size.value();
	return bufferVec;
}