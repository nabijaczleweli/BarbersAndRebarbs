// The MIT License (MIT)

// Copyright (c) 2014 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include "file.hpp"
#include <cstdio>
#include <dirent.h>

using namespace std;


// http://stackoverflow.com/questions/612097/how-can-i-get-a-list-of-files-in-a-directory-using-c-or-c
vector<string> list_files(string directory) {
	DIR * dir = nullptr;
	dirent * ent = nullptr;
	vector<string> result;
	if((dir = opendir(directory.c_str())) != nullptr) {
		/* print all the files and directories within directory */
		while((ent = readdir(dir)) != nullptr)
			result.emplace_back(ent->d_name);
		closedir(dir);
		dir = nullptr;
		ent = nullptr;
		result.shrink_to_fit();
	}
	return result;
}

bool file_exists(const std::string & path) {
	if(FILE * file = fopen(path.c_str(), "r")) {
		fclose(file);
		return true;
	} else
		return false;
}
