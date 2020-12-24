#include "filehasher_delegate.h"
#include "../../Utility/FileUtility.h"

size_t FileHasherDelegate::GetFileSize(QString filePath)
{
    return FileUtil::GetFileSize(reinterpret_cast<const wchar_t*>(filePath.unicode()));
}

QString FileHasherDelegate::CreateHash(QString filePath, HashingAlgorithm *hashAlgo)
{
    return QString::fromStdString(hashAlgo->CalculateHash(reinterpret_cast<const wchar_t*>(filePath.unicode())));
}
