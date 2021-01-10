# FileHasher
Create and compare hashes of files and / or text.  
Generate the hashes of the files you want (e.g. .exe and .dll) and compare this hash to the one generated at runtime.  
By doing so you can detect updates for any game / software without relying on strings or version numbers inside the file. This approach also never touches the file in memory, it only reads it from the disk.  
  
[![Release](https://img.shields.io/github/v/release/AlEscher/FileHasher?color=light-green&include_prereleases&style=plastic)](https://github.com/AlEscher/FileHasher/releases/latest) [![GitHub](https://img.shields.io/github/license/AlEscher/FileHasher?color=cyan&style=plastic)](https://github.com/AlEscher/FileHasher/blob/master/LICENSE)

## Speed:  
In order to know what to expect in terms of performance, I did some tests for SHA256 with a blocksize of 16777216 (2^24) Bytes. 
SHA512 will be significantly faster, since we only have to do half of the iterations that SHA256 does. 
There is no real reason to use a larger blocksize, however other sizes may perform better on different hardware.  
These measurements were done with the release version of the Console file, the GUI will be slower (more threads, synchronization, etc...)
| FileSize (KB) | Runtime (ms) |
|:-------------:|:------------:|
| 14847574      | 54654        |
| 5186093       | 19142        |
| 3770744       | 13941        |
| 1039630       | 3833         |
| 431357        | 1615         |
| 103972        | 389          |
| 1245          | 5.1          |
| 115           | 0.7          |  

## GUI:  
![GUI_Preview](https://github.com/AlEscher/FileHasher/blob/master/GUI/FileHasherUI/Resources/GUIPreview.PNG)

## Known Issues:  
- File progress bar breaks with files > 2GB, this is because QProgressBar only takes signed integers  

## Sources:
The only sources used are:
- [SHA256](https://en.wikipedia.org/wiki/SHA-2)
- [SHA RFC](https://tools.ietf.org/html/rfc3174)
- Sources included in code comments

## External
I am using [Qt](https://www.qt.io/download-open-source) for the GUI
and [QtFramelessWindowDarkStyle](https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle) for the dark mode.  
Download the [Qt Installer](https://www.qt.io/download-qt-installer) and install version 15.5.0, add the project files in [FileHasherUI](https://github.com/AlEscher/FileHasher/tree/master/GUI/FileHasherUI) and you should be good to go  
(you will need to keep the whole project's folder structure, or make appropiate changes to the [.pro file](https://github.com/AlEscher/FileHasher/blob/master/GUI/FileHasherUI/FileHasherUI.pro)).  
