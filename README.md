# Project Title

This app searches for a text in all filese from a give path

## Getting Started

In practice you should clone the repo and load the solution in VS 2017, copile it and run it

## Solution I made

I am iterating the path and search for files
Once I get the files I send them to my searcInFiles thread
The thread is an active object consuming a queue with functions
In my case the functions are my search function
The search function maps a file into memory and searches for the "needle" untill the end of the file
Once I have the index, I just print the values:file, offset, prefix, sufix

The main file is as light as possible. I should have made a special class for the search function and for the print function
I am also not to satisfied on the prints :) But that's details.

### Prerequisites

You shouldn't need to install anything before. Just compile and run the exe. Be sure to provide the proper params

## Tests made

I tested the app on win10 x64 - x64 version - so at least this should work fine
For win32 I don't map more than 1GB of large files
I tested this on c:/windows without any crashes and with good results
The maximum file size I tested it was 2.7GB and contained only one line with the needle at the end

## Authors

* **Claudiu Lupu** - *Initial work* 