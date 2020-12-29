# FileHasher
Create and compare hashes of files and / or text.  
Generate the hashes of the files you want (e.g. .exe and .dll) and compare this hash to the one generated at runtime.  
By doing so you can detect updates for any game / software without relying on strings or version numbers inside the file. This approach also never touches the file in memory, it only reads it from the disk.  

## Speed:  
In order to know what to expect in terms of performance, I did some tests with different blocksizes.  
As can be seen, 16 MB has the best overall performance:  
###  16777216 (2^24) Bytes |  4096 (2^12) Bytes   | 4294967296 (2^32) Bytes
| FileSize (KB) | Runtime (ms) |     FileSize (KB) | Runtime (ms) |     FileSize (KB) | Runtime (ms) |
|:-------------:|:------------:|    :-------------:|:------------:|    :-------------:|:------------:|
| 14847574      | 60012        |     14847574      | 67985        |     14847574      | 56566        |
| 5186093       | 19041        |     5186093       | 23678        |     5186093       | 19465        |
| 3770744       | 13883        |     3770744       | 17276        |     3770744       | 14608        |
| 1039630       | 3829         |     1039630       | 4771         |     1039630       | 4205         |
| 431357        | 1588         |     431357        | 1963         |     431357        | 1855         |
| 103972        | 385          |     103972        | 475          |     103972        | 611          |
| 1245          | 6.7          |     1245          | 6.1          |     1245          | 219          |
| 115           | 2.7          |     115           | 0.8          |     115           | 215          |

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
