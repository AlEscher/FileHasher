#pragma once
#include <vector>
#include <optional>

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
	static std::optional<size_t> GetFileSizeW(const wchar_t* filePath);
	// Wrapper around GetFileSizeW
	static std::optional<size_t> GetFileSizeA(const char* filePath);
	// Reads the specified file into memory, and sets fileSize to the size of the file in bytes
	// fileSize will be set to 0 if anything fails
	static std::vector<uint8_t> ReadFileContent(const wchar_t* filePath, size_t& fileSize);

	FileUtil();
	// Specify a custom blocksize in Bytes that GetNextBlock will read, default is 4096 bytes
	FileUtil(size_t blockSize);
	~FileUtil();
	// Opens the specified file stream in binary mode, and keeps it open until this object is deleted
	bool OpenFileStreamW(const wchar_t* filePath);
	// Return whether we can read anymore from our input stream,
	// checks if the file stream is open and available, and that there is still data to be read
	bool CanRead() const;
	// Checks whether the file stream is open and error free
	bool IsOpen() const;
	// Read the next block from the file, returns a heap allocated pointer, nullptr if anything fails
	uint8_t* GetNextBlock();
	// Returns how many bytes we still have to read
	size_t BytesRemaining() const;
	// Close the file stream and reset internal values
	void Reset();
	inline size_t GetBlockSize() const
	{
		return this->m_nBlockSize;
	}
};