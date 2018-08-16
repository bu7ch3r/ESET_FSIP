// ESET_FSIP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::experimental::filesystem;

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

		if (!is_directory(thePath))
		{
			cout << thePath.filename() << "\n";
		}
		else
		{
			for (fs::recursive_directory_iterator i(argv[1]), end; i != end; ++i)
				if (!fs::is_directory(i->path()))
					cout << i->path().filename() << "\n";
		}
	}
    return 0;
}

