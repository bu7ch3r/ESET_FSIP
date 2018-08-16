// ESET_FSIP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <framework/Active.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono> 

using namespace std;
namespace fs = std::experimental::filesystem;

void FindInFile(const string& searchPath, const string& needle)
{
	ifstream fileInput;
	size_t offset;
	string line;

	fileInput.open(searchPath.c_str());
	if (fileInput.is_open())
	{
		while (getline(fileInput, line))
		{
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
						cout << "Exception for at \n";
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
						cout << "Exception for at \n";
					}
				}
			}
		}
		fileInput.close();
	}
}

int main(int argc, char *argv[])
{
	Active activeObject;

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
			FindInFile(thePath.generic_string(), needle);
		}
		else
		{
			for (fs::recursive_directory_iterator i(argv[1]), end; i != end; ++i)
			{
				if (!fs::is_directory(i->path()))
				{
					std::string path = i->path().generic_string();
					activeObject.Send([=] {FindInFile(std::move(path), needle);});
				}
			}
		}
	}	
    return 0;
}

