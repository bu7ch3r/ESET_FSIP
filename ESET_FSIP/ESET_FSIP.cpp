// ESET_FSIP.cpp : Defines the entry point for the console application.
//
#include <filesystem>
#include <framework/Active.h>
#include <framework/MemoryMapped.h>
#include <iostream>
#include <string>
#include <string_view>

using namespace std;
namespace fs = std::experimental::filesystem;

void printEscaped(unsigned char printChar)
{
	if ('\t' == printChar)
		cout << "\\t";
	else if ('\n' == printChar)
		cout << "\\n";
	else if ('\r' == printChar)
		cout << "\\r";
	else
		cout << printChar;
}

void printSolution(const MemoryMapped& data, const size_t xNeedlePos, const size_t xNeedleLength)
{
	cout << data.fileName() << "(" << xNeedlePos << "):";
	
	if (xNeedlePos >= 3)
		printEscaped(data.at(xNeedlePos - 3));
	if (xNeedlePos >= 2)
		printEscaped(data.at(xNeedlePos - 2));
	if (xNeedlePos >= 1)
		printEscaped(data.at(xNeedlePos - 1));

	cout << "...";

	try
	{
		printEscaped(data.at(xNeedlePos + xNeedleLength));
		printEscaped(data.at(xNeedlePos + xNeedleLength + 1));
		printEscaped(data.at(xNeedlePos + xNeedleLength + 2));
	}
	catch (...)
	{
		//what to do? nothing!
	}
	cout << "\n";
}

void FindInMappedFile(const string& searchPath, const string& needle)
{
	MemoryMapped data(searchPath, MemoryMapped::WholeFile, MemoryMapped::Normal);

	if (!data.isValid())
	{
		cout << "File " << searchPath << " cannot be mapped" << endl;
		return;
	}

	char* buffer = (char*)data.getData();
	
	if (buffer)
	{
		auto dataSize = data.size();
		string_view bufferView{ buffer, dataSize };
		size_t xSearchPos = 0;
		size_t xNeedleLength = needle.size();
		size_t xNeedlePos = string::npos;

		do
		{
			xNeedlePos = bufferView.find(needle, xSearchPos);

			if (string::npos != xNeedlePos)
			{
				printSolution(data, xNeedlePos, xNeedleLength);
				xSearchPos = xNeedlePos + xNeedleLength;
			}			
		} while (string::npos != xNeedlePos);
	}
}

int main(int argc, char *argv[])
{
	Active activeFileSearcher;

	if (3 != argc)
	{
		cout << "Usage: [path] [search_string]" << endl;
		return 1;
	}

#ifndef _WIN64
	cout << "The application is not ready yet for x86 platform";
	return 1;
#endif

	if (128 < strlen(argv[2]))
	{
		cout << "Search string is to long and will be truncated to 128 chars\n";
		argv[2][128] = '\0';
	}
	
	if (!fs::exists(argv[1]))
	{
		cout << "Path doesn't exist\n";
	}
	else
	{	
		fs::path thePath(argv[1]);
		string needle(argv[2]);
		if (!is_directory(thePath))
		{
			FindInMappedFile(thePath.generic_string(), needle);
		}
		else
		{
			for (fs::recursive_directory_iterator i(argv[1]), end; i != end; ++i)
			{
				if (!fs::is_directory(i->path()))
				{
					std::string path = i->path().generic_string();
					activeFileSearcher.Send([=]{FindInMappedFile(std::move(path), needle);});
				}
			}
		}
	}	
    return 0;
}

