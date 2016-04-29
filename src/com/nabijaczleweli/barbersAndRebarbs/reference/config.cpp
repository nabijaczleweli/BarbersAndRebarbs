// The MIT License (MIT)

// Copyright (c) 2016 nabijaczleweli

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


#include "config.hpp"


using namespace std;


config::config(string path) : path(path) {
	// TODO: cereal load
/*
	if(cfg.contains("system:language"))
		const_cast<string &>(app_language) = cfg.get("system:language").textual();
	else {
		string files;
		for(const auto & name : list_files(localization_root))
			if(name[0] != '.' && name.size() == 10 && name.find(".lang") == 5)
				files += name.substr(0, name.find(".lang")) + ", ";
		cfg.get("system:language", property(app_language, "Available languages: " + files.substr(0, files.size() - 2)));
	}*/
}

config::~config() {
	// TODO: cereal save
}
