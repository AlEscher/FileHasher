#ifndef FILEHASHERDELEGATE_H
#define FILEHASHERDELEGATE_H
#include <QString>
#include "filehasher.h"

#include "../../Hashing/HashingAlgorithm.h"

class Worker;

// Delegates calls from the UI to our backend
class FileHasherDelegate
{
private:
    Worker* worker = nullptr;
public:
    // Get the file size from a string, for example 123 from "123 KB"
    size_t GetSizeFromString(const QString& string);
    QString GetHashFromString(const QString& string);
    // Get the size in bytes of the specified file
    size_t GetFileSize(const QString& filePath);
    // Check if the specified element exists and is accessible
    bool CheckFilePath(const QString& filePath);
    QString CreateHash(const QString&, HashingAlgorithm* hashAlgo);
    bool SetClipboardText(const QString& text);
    QString GetClipboardText();
    inline void ResetHashingAlgorithm(HashingAlgorithm* hashAlgo)
    {
        if (hashAlgo)
        {
            hashAlgo->Reset();
        }
    }
};

#endif // FILEHASHERDELEGATE_H
