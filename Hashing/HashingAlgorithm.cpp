#include "./HashingAlgorithm.h"

using namespace std;

string HashingAlgorithm::CalculateFileHash(const wchar_t* filePath)
{
	size_t fileSize = 0;
	fileSize = FileUtil::GetFileSizeW(filePath);
	if (fileSize == 0)
	{
		return "";
	}

	if (!m_pFileUtil->OpenFileStreamW(filePath) || !m_pFileUtil->CanRead())
	{
		return "";
	}

	string hash = Hash(fileSize);
	m_pFileUtil->Reset();
	return hash;
}

std::string HashingAlgorithm::CalculateFileHash(const char* filePath)
{
	size_t fileSize = 0;
	fileSize = FileUtil::GetFileSizeA(filePath);
	if (fileSize == 0)
	{
		return "";
	}

	if (!m_pFileUtil->OpenFileStreamA(filePath) || !m_pFileUtil->CanRead())
	{
		return "";
	}

	string hash = Hash(fileSize);
	m_pFileUtil->Reset();
	return hash;
}

string HashingAlgorithm::CalculateFileHash(const wchar_t* filePath, size_t& fileSize)
{
	size_t fileSizeLoc = fileSize = 0;
	fileSizeLoc = FileUtil::GetFileSizeW(filePath);
	if (fileSizeLoc == 0)
	{
		return "";
	}

	fileSize = fileSizeLoc;
	return CalculateFileHash(filePath);
}