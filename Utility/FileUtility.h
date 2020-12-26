#pragma once
#include <vector>

class FileUtil
{
private:
	FILE* m_pInput = nullptr;
	bool m_bIsOpen = false;
	size_t m_nBlockSize = 4096U;
	size_t m_nTotalFileSize = 0U;
public:
	// Get the size of a file on disk.
	// Returns file size on success, 0 otherwise
	static size_t GetFileSizeW(const wchar_t* filePath);
	// Wrapper around GetFileSizeW
	static size_t GetFileSizeA(const char* filePath);
	// Reads the specified file into memory, and sets fileSize to the size of the file in bytes
	// fileSize will be set to 0 if anything fails
	static std::vector<uint8_t> ReadFileContent(const wchar_t* filePath, size_t& fileSize);

	FileUtil();
	// Specify a custom blocksize in Bytes that GetNextBlock will read, default is 4096 bytes
	FileUtil(size_t blockSize);
	~FileUtil();
	// Opens the specified file stream in binary mode, and keeps it open until this object is deleted
	bool OpenFileStreamA(const char* filePath);
	// Wrapper for OpenFileStreamA
	bool OpenFileStreamW(const wchar_t* filePath);
	// Return whether we can read anymore from our input stream,
	// checks if the file stream is open and available, and that we didn't reach EOF
	bool CanRead();
	// Read the next block from the file, returns a heap allocated pointer, nullptr if anything fails
	uint8_t* GetNextBlock();
	// Returns how many bytes we still have to read
	size_t BytesRemaining();
	// Close the file stream and reset internal values
	void Reset();
	inline size_t GetBlockSize()
	{
		return this->m_nBlockSize;
	}
};