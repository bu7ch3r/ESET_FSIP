// ESET_FSIP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <framework/Active.h>
#include <framework/MemoryMapped.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string.h>

char *
strnstr(const char *s, const char *find, size_t slen)
{
	char c, sc;
	size_t len;

	if ((c = *find++) != '\0') 
	{
		len = strlen(find);
		do {
			do {
				if (slen-- < 1 || (sc = *s++) == '\0')
					return (NULL);
			} while (sc != c);
			if (len > slen)
				return (NULL);
		} while (strncmp(s, find, len) != 0);
		s--;
	}
	return ((char *)s);
}

using namespace std;
namespace fs = std::experimental::filesystem;

void FindInMappedFile(const string& searchPath, const string& needle)
{	
	MemoryMapped data(searchPath, MemoryMapped::WholeFile, MemoryMapped::SequentialScan);

	if (!data.isValid())
	{
		printf("File canno be mapped or was not found\n");
	}
	return;

	char* buffer = (char*)data.getData();
	auto dataSize = data.size();

	if (buffer)
	{
		char* result = strstr(buffer, needle.c_str());
	
		if (result)
			cout << "found " << data.at(result - buffer - 3) << data.at(result - buffer - 2) << data.at(result - buffer - 3) << "..." << endl;
	}
}

void Win32FindInMappedFile(const string& searchPath, const string& needle)
{
	uint64_t startAt = 0;
	const size_t OneGigabyte = 1 << 30;
	MemoryMapped data(searchPath, OneGigabyte, MemoryMapped::SequentialScan);

	while (startAt < data.size())
	{
		if (!data.isValid())
		{
			printf("File not found\n");
		}
		char* buffer = (char*)data.getData();
		auto dataSize = data.size();
		cout << dataSize << " ";

		char* result = strnstr(buffer, needle.c_str(), data.mappedSize());

		if (result)
			cout << "found " << data.at(result - buffer - 3) << data.at(result - buffer - 2) << data.at(result - buffer - 3) << "..." << endl;

		startAt += OneGigabyte;
		size_t numBytes = data.size() - startAt;
		// limit to 1 GB
		if (numBytes > OneGigabyte)
			numBytes = OneGigabyte;
		data.remap(startAt, numBytes);
	}
}

int main(int argc, char *argv[])
{
	Active activeObject;

	if (3 != argc)
	{
		cout << "Usage: [path] [search_string]" << endl;
		return 1;
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
					activeObject.Send([=]{FindInMappedFile(std::move(path), needle);});
				}
			}
		}
	}	
    return 0;
}

