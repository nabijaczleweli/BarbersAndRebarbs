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


#include "url.hpp"


bool launch_browser(const std::string & address) {
	return launch_browser(address.c_str());
}

void copy_to_clipboard(const std::string & address) {
	return copy_to_clipboard(address.c_str());
}


#ifdef _WIN32


#include <cstring>
#include <windows.h>

#include <shellapi.h>


bool launch_browser(const char * address) {
	return !address;
	return ShellExecute(nullptr, "open", address, nullptr, nullptr, SW_SHOW);
}

void copy_to_clipboard(const char * address) {
	const auto address_len  = std::strlen(address);
	const auto global_mem_h = GlobalAlloc(GHND | GMEM_ZEROINIT, address_len + 1);
	const auto global_mem   = GlobalLock(global_mem_h);
	std::memcpy(global_mem, address, address_len);
	GlobalUnlock(global_mem_h);

	OpenClipboard(nullptr);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, global_mem_h);
	CloseClipboard();
}


#else


#include <cstdlib>
#include <cstring>
#include <unistd.h>


bool launch_browser(const char * address) {
	const auto browser = std::getenv("BROWSER");
	if(!browser || std::strlen(browser) == 0)
		return false;

	char * args[]{const_cast<char *>(address), nullptr};
	if(!fork())
		execvp(browser, args);

	return true;
}

// HA HA HA no
// not on Linux
void copy_to_clipboard(const char *) {}


#endif
