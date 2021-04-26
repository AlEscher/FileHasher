#include <chrono>
#include <iostream>
#include <sstream>

#include "./Hashing/HashingAlgorithm.h"
#include "./Hashing/MD5Hash.h"
#include "./Hashing/SHA1Hash.h"
#include "./Hashing/SHA256Hash.h"
#include "./Hashing/SHA512Hash.h"

using namespace std;

string GetProgramName(const string& pathToProgram)
{
    string programName;
    vector<string> elements;
    istringstream f(pathToProgram);
    // Get program name from command line argument containing program path
    while (getline(f, programName, '\\'))
    {
    }

    return programName;
}

void PrintHelp(const string& pathToProgram)
{
    const string programName = GetProgramName(pathToProgram);
    cout << "Program usage:" << endl;
    cout << "\t- " << programName << " <HashAlgorithm> <PathToFile>" << endl;
}

int HandleCommandLineInput(const int argc, char** argv)
{
    if (argc < 3)
    {
        PrintHelp(argv[0]);
        return 1;
    }

    vector<string> arguments;
    arguments.reserve(argc);
    for (int i = 0; i < argc; i++)
    {
        arguments.emplace_back(argv[i]);
    }

    bool verbose = false;
    if (const auto verbosePosition = find(arguments.begin(), arguments.end(), "-v"); verbosePosition != arguments.end())
    {
        verbose = true;
    }

    HashingAlgorithm* algorithm;
    if (arguments[1] == "SHA256")
    {
        algorithm = new SHA256Hasher();
    }
    else if (arguments[1] == "SHA512")
    {
        algorithm = new SHA512Hasher();
    }
    else if (arguments[1] == "MD5")
    {
        algorithm = new MD5Hasher();
    }
    else if (arguments[1] == "SHA1")
    {
        algorithm = new SHA1Hasher();
    }
    else
    {
        if (verbose)
        {
            cout << "Invalid Hashing Algorithm!" << endl;
            PrintHelp(arguments[0]);
        }
        return 1;
    }

    const size_t len = strnlen_s(arguments[2].c_str(), 1024U) + 1;
    [[maybe_unused]] size_t fileSize = 0U;
    size_t ret = 0U;
    auto* widePath = new wchar_t[len];
    mbstowcs_s(&ret, widePath, len, arguments[2].c_str(), len - 1);

    const string hash = algorithm->CalculateFileHash(widePath);
    delete[] widePath;
    if (hash.empty())
    {
        if (verbose)
        {
            cout << "Invalid file path!" << endl;
            PrintHelp(arguments[0]);
        }
        return 1;
    }

    cout << hash;

    return 0;
}

int main(int argc, char** argv)
{
    HashingAlgorithm* sha256Hasher = new SHA256Hasher();
    HashingAlgorithm* sha512Hasher = new SHA512Hasher();
    HashingAlgorithm* md5Hasher = new MD5Hasher();
    HashingAlgorithm* sha1Hasher = new SHA1Hasher();
    wstring path = L"C:/Dev/Cpp/FileHasher/test2";

    if (argc > 1)
    {
        return HandleCommandLineInput(argc, argv);
    }

    while (true)
    {
        string input;
        cin >> input;

        if (input == "exit")
        {
            break;
        }
    	
        if (input == "-path")
        {
	        cout << "Enter path: ";
	        wcin >> path;
        }
        else if (input == "file")
        {
	        /*
            * Tested files (Release mode), times include reading file and hashing,
            * so this may vary depending on harddrive (NVMe SSD for me). Tested only once:
            * Size (using filesystem::file_size / 1000) and duration (using chrono::high_resolution_clock) :
            * - 14847574 KB     => 134895 ms
            * - 5186093 KB      => 45846 ms     (27.9s were spent with File IO, and 17.9s with hashing)
            * - 1039630 KB      => 9304 ms      (5.7s were spent with File IO, and 3.6s with hashing)
            * - 431357 KB       => 3820 ms      (2.3s were spent with File IO, and 1.5s with hashing)
            * - 103972 KB       => 928 ms
            * - 1245 KB         => 11 ms
            * - 115 KB          => 1 ms
            * 
            * New results after runtime and memory improvement
            * (Tested multiple times, if results didn't vary too much first result was taken):
            * Blocksize used: 16777216 (2^24) Bytes
            * - 14847574 KB     => 60012 ms
            * - 5186093 KB      => 19041 ms
            * - 3770744 KB      => 13883 ms
            * - 1039630 KB      => 3829 ms
            * - 431357 KB       => 1588 ms
            * - 103972 KB       => 385 ms
            * - 1245 KB         => 6 ms (6768 microseconds)
            * - 115 KB          => 2 ms (2494 microseconds)
            * 
            * Blocksize used: 4096 (2^12) Bytes
            * - 14847574 KB     => 67985 ms
            * - 5186093 KB      => 23678 ms
            * - 3770744 KB      => 17276 ms
            * - 1039630 KB      => 4771 ms
            * - 431357 KB       => 1963 ms
            * - 103972 KB       => 475 ms
            * - 1245 KB         => 6 ms (6062 microseconds)
            * - 115 KB          => 0 ms (812 microseconds)
            * 
            * Blocksize used: 4294967296 (2^32) Bytes
            * - 14847574 KB     => 56566 ms
            * - 5186093 KB      => 19465 ms
            * - 3770744 KB      => 14608 ms
            * - 1039630 KB      => 4205 ms
            * - 431357 KB       => 1855 ms
            * - 103972 KB       => 611 ms
            * - 1245 KB         => 219 ms
            * - 115 KB          => 215 ms
            */

	        // Hash the file "test", can be changed at runtime
	        size_t fileSize = 0;
	        auto start = chrono::high_resolution_clock::now();
	        string sha256Hash = sha256Hasher->CalculateFileHash(path.c_str(), fileSize);
	        auto end = chrono::high_resolution_clock::now();
	        cout << "File " << sha256Hasher->GetName() << ": " << sha256Hash << endl;
	        cout << "Took: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms, with size: " << fileSize / 1000 << " KB" << endl;
	        cout << "Took: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " micros, with size: " << fileSize / 1000 << " KB" << endl;

	        start = chrono::high_resolution_clock::now();
	        string sha512Hash = sha512Hasher->CalculateFileHash(path.c_str());
	        end = chrono::high_resolution_clock::now();
	        cout << "File " << sha512Hasher->GetName() << ": " << sha512Hash << endl;
	        cout << "Took: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms " << endl;
	        cout << "Took: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " micros" << endl;

	        start = chrono::high_resolution_clock::now();
	        string md5Hash = md5Hasher->CalculateFileHash(path.c_str());
	        end = chrono::high_resolution_clock::now();
	        cout << "File MD5:    " << md5Hash << endl;
	        cout << "Took: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms, with size: " << fileSize / 1000 << " KB" << endl;

	        start = chrono::high_resolution_clock::now();
	        string sha1Hash = sha1Hasher->CalculateFileHash(path.c_str());
	        end = chrono::high_resolution_clock::now();
	        cout << "File SHA1:   " << sha1Hash << endl;
	        cout << "Took: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms, with size: " << fileSize / 1000 << " KB" << endl;
        }
        else
        {
	        cout << "Unknown command: " << input << endl;
        }
    }

    delete sha256Hasher;
    delete sha512Hasher;
    delete md5Hasher;
    delete sha1Hasher;
}