#include "FileHasherDelegate.h"

#include <QFile>
#include <QGuiApplication>
#include <QClipboard>

size_t FileHasherDelegate::GetFileSize(const QString& filePath)
{
    return FileUtil::GetFileSizeW(reinterpret_cast<const wchar_t*>(filePath.unicode())).value_or(0U);
}

QString FileHasherDelegate::CreateHash(const QString& filePath, HashingAlgorithm *hashAlgo)
{
    return QString::fromStdString(hashAlgo->CalculateFileHash(reinterpret_cast<const wchar_t*>(filePath.unicode())));
}

bool FileHasherDelegate::CheckFilePath(const QString& filePath)
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

size_t FileHasherDelegate::GetSizeFromString(const QString& string)
{
    QStringList list = string.split(" ");
    if (list.size() > 0)
    {
        return list.at(0).toUInt();
    }
    else
    {
        return 0U;
    }
}

QString FileHasherDelegate::GetHashFromString(const QString& string)
{
    QStringList splitStrings = string.split(' ');
    size_t size = splitStrings.size();
    // Hash will always be the last element
    return size > 0U ? splitStrings[size - 1U] : "";
}

bool FileHasherDelegate::SetClipboardText(const QString &text)
{
    QClipboard* clipboard = QGuiApplication::clipboard();
    if (clipboard)
    {
        clipboard->clear();
        clipboard->setText(text, QClipboard::Clipboard);
        return true;
    }

    return false;
}

QString FileHasherDelegate::GetClipboardText()
{
    if(QClipboard* clipboard = QGuiApplication::clipboard())
    {
        return clipboard->text(QClipboard::Clipboard);
    }
    return "";
}
