// ESET_FSIP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;
namespace fs = std::experimental::filesystem;

void FindInFile(const fs::path & searchPath, const string& needle)
{
	ifstream fileInput;
	size_t offset;
	string line;
	
	fileInput.open(searchPath.c_str());
	if (fileInput.is_open()) 
	{
		while (!fileInput.eof())
		{
			getline(fileInput, line);
			if ((offset = line.find(needle, 0)) != string::npos)
			{
				cout << "found: " << needle << " at offset " << offset << endl;
				for (int i = 3; i >= 1; i--)
				{
					try
					{
						cout << line.at(offset - i);
					}
					catch (...)
					{

					}
				}
				cout << "...";
				for (int i = 1; i <= 3; i++)
				{
					try
					{
						cout << line.at(offset + needle.size() + i);
					}
					catch (...)
					{

					}
				}
			}
		}
		fileInput.close();
	}
}

int main(int argc, char *argv[])
{
	if (3 != argc)
	{
		cout << "Usage: [path] [search_string]" << endl;
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
			FindInFile(thePath, needle);
		}
		else
		{
			for (fs::recursive_directory_iterator i(argv[1]), end; i != end; ++i)
			{
				if (!fs::is_directory(i->path()))
				{
					FindInFile(i->path(), needle);
				}
			}
		}
	}
    return 0;
}

