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
    size_t GetSizeFromString(QString string);
    QString GetHashFromString(QString string);
    // Get the size in bytes of the specified file
    size_t GetFileSize(QString filePath);
    // Check if the specified element exists and is accessible
    bool CheckFilePath(QString filePath);
    QString CreateHash(QString filePath, HashingAlgorithm* hashAlgo);
    inline void ResetHashingAlgorithm(HashingAlgorithm* hashAlgo)
    {
        if (hashAlgo)
        {
            hashAlgo->Reset();
        }
    }
};

#endif // FILEHASHERDELEGATE_H
