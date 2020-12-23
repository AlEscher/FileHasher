#include "filehasher_delegate.h"
#include "../../Utility/FileUtility.h"

size_t FileHasherDelegate::GetFileSize(QString filePath)
{
    return FileUtil::GetFileSize(reinterpret_cast<const wchar_t*>(filePath.unicode()));
}
