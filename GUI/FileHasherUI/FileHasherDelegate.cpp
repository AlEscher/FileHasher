#include "FileHasherDelegate.h"

size_t FileHasherDelegate::GetFileSize(QString filePath)
{
    return FileUtil::GetFileSizeW(reinterpret_cast<const wchar_t*>(filePath.unicode()));
}

QString FileHasherDelegate::CreateHash(QString filePath, HashingAlgorithm *hashAlgo)
{
    return QString::fromStdString(hashAlgo->CalculateFileHash(reinterpret_cast<const wchar_t*>(filePath.unicode())));
}
