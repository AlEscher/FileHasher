#ifndef FILEHASHER_DELEGATE_H
#define FILEHASHER_DELEGATE_H

#include <QString>

// Delegates calls from the UI to our backend
class FileHasherDelegate
{
public:
    // Get the size in bytes of the specified file
    size_t GetFileSize(QString filePath);
};

#endif // FILEHASHER_DELEGATE_H
