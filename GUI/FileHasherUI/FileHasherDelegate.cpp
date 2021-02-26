#include "FileHasherDelegate.h"

#include <QFile>

size_t FileHasherDelegate::GetFileSize(QString filePath)
{
    return FileUtil::GetFileSizeW(reinterpret_cast<const wchar_t*>(filePath.unicode()));
}

QString FileHasherDelegate::CreateHash(QString filePath, HashingAlgorithm *hashAlgo)
{
    return QString::fromStdString(hashAlgo->CalculateFileHash(reinterpret_cast<const wchar_t*>(filePath.unicode())));
}

bool FileHasherDelegate::CheckFilePath(QString filePath)
{
    // Check if the file exists
    if (!QFile::exists(filePath))
    {
        return false;
    }

    // Check that we can access it
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    return true;
}
