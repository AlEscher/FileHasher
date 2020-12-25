# FileHasher
Create and compare hashes of files and / or text.  
Generate the hashes of the files you want (e.g. .exe and .dll) and compare this hash to the one generated at runtime.  
By doing so you can detect updates for any game / software without relying on strings or version numbers inside the file. This approach also never touches the file in memory, it only reads it from the disk.  

## TODO:
- [Planned features](https://github.com/AlEscher/FileHasher/projects/1)

## Sources:
The only sources used are:
- [SHA256](https://en.wikipedia.org/wiki/SHA-2)
- [SHA RFC](https://tools.ietf.org/html/rfc3174)
- Sources included in code comments

## External
I am using [Qt](https://www.qt.io/) for the GUI
and [QDarkStyleSheet](https://github.com/ColinDuquesnoy/QDarkStyleSheet) for the dark mode
