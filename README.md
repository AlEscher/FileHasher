# FileHasher
Create and compare hashes of files and / or text.  
Generate the hashes of the files you want (e.g. .exe and .dll) and compare this hash to the one generated at runtime.  
By doing so you can detect updates for any game / software without relying on strings or version numbers inside the file. This approach also never touches the file in memory, it only reads it from the disk.  

## Speed:  
In order to know what to expect in terms of performance, I did some tests with a blocksize of 16777216 (2^24) Bytes:  
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

There is no real reason to use a larger blocksize, however other sizes may perform better on different hardware.  

## TODO:
- [Planned features](https://github.com/AlEscher/FileHasher/projects/1)

## Sources:
The only sources used are:
- [SHA256](https://en.wikipedia.org/wiki/SHA-2)
- [SHA RFC](https://tools.ietf.org/html/rfc3174)
- Sources included in code comments

## External
I am using [Qt](https://www.qt.io/) for the GUI
and [QtFramelessWindowDarkStyle](https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle) for the dark mode
