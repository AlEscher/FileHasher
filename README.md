# FileHasher
Create and compare hashes of files and / or text.  
Generate the hashes of the files you want (e.g. .exe and .dll) and compare this hash to the one generated at runtime.  
By doing so you can detect updates for any game / software without relying on strings or version numbers inside the file.  
This approach also never touches the file in memory, it only reads it from the disk.  

## TODO:
- GUI client
- Includable header file, DLL, or lib

## Sources:
The only sources used are:
- https://en.wikipedia.org/wiki/SHA-2 (SHA256)
- https://tools.ietf.org/html/rfc3174 (SHA256)
- Sources included in code comments
