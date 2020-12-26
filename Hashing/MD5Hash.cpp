#include <cstdint>
#include <windows.h>
#include <Wincrypt.h>

constexpr auto BUFSIZE = 1024;
constexpr auto MD5LEN = 16;

#include "MD5Hash.h"
#include "../Utility/FileUtility.h"


// As specified by https://docs.microsoft.com/en-us/windows/win32/seccrypto/example-c-program--creating-an-md-5-hash-from-file-content

std::string MD5Hasher::CalculateFileHash(const wchar_t* filePath)
{
	uint32_t dwStatus = 0;
	bool bResult = false;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	HANDLE hFile = NULL;
	BYTE rgbFile[BUFSIZE];
	DWORD cbRead = 0;
	BYTE rgbHash[MD5LEN];
	DWORD cbHash = 0;
	CHAR rgbDigits[] = "0123456789abcdef";

	hFile = CreateFile(filePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return "";
	}

	// Get handle to the crypto provider
	if (!CryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		CloseHandle(hFile);
		return "";
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
	{
		CloseHandle(hFile);
		CryptReleaseContext(hProv, 0);
		return "";
	}

	while (bResult = ReadFile(hFile, rgbFile, BUFSIZE, &cbRead, NULL))
	{
		if (0 == cbRead)
		{
			break;
		}

		if (!CryptHashData(hHash, rgbFile, cbRead, 0))
		{
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			CloseHandle(hFile);
			return "";
		}
	}

	if (!bResult)
	{
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		CloseHandle(hFile);
		return "";
	}

	cbHash = MD5LEN;
	char* md5Hash = nullptr;
	if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
	{
		// +1 for Null termination
		md5Hash = new char[(unsigned long long)cbHash * 2 * sizeof(char) + 1];
		if (!md5Hash)
		{
			CryptDestroyHash(hHash);
			CryptReleaseContext(hProv, 0);
			CloseHandle(hFile);
			return "";
		}

		// Create our hash
		for (DWORD i = 0; i < cbHash; i++)
		{
			md5Hash[i * 2] = rgbDigits[rgbHash[i] >> 4];
			md5Hash[i * 2 + 1] = rgbDigits[rgbHash[i] & 0xf];
		}
		// Null terminate
		md5Hash[cbHash * 2] = 0;
	}
	else
	{
		// Nothing todo, md5Hash will be NULL which will be handled by caller
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	CloseHandle(hFile);

	return (md5Hash != nullptr ? std::string(md5Hash) : "");
}

std::string MD5Hasher::CalculateFileHash(const char* filePath)
{
	return std::string();
}

std::string MD5Hasher::CalculateFileHash(const wchar_t* filePath, size_t& fileSize)
{
	std::string hash = this->CalculateFileHash(filePath);
	fileSize = FileUtil::GetFileSizeW(filePath);
	return hash;
}

std::string MD5Hasher::CalculateStringHash(const std::string& input)
{
	return "";
}