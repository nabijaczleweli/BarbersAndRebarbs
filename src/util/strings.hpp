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


#pragma once
#ifndef STRINGS_HPP
#define STRINGS_HPP


#include <algorithm>
#include <cctype>
#include <functional>
#include <locale>


// Stolen from http://stackoverflow.com/a/217605/2851815
static inline std::string & ltrim(std::string & s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// Stolen from http://stackoverflow.com/a/217605/2851815
static inline std::string & rtrim(std::string & s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// Stolen from http://stackoverflow.com/a/217605/2851815
static inline std::string & trim(std::string & s) {
	return ltrim(rtrim(s));
}

static inline std::string & ltrim(std::string && s) {
	return ltrim(s);
}

static inline std::string & rtrim(std::string && s) {
	return rtrim(s);
}

static inline std::string & trim(std::string && s) {
	return trim(s);
}


#endif  // STRINGS_HPP
