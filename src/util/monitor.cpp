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


#include "monitor.hpp"


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


unsigned int refresh_rate() {
	DEVMODE dmode{};
	dmode.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dmode);
	return dmode.dmDisplayFrequency;
}
#else
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>


unsigned int refresh_rate() {
	auto dpy  = XOpenDisplay(nullptr);
	auto root = RootWindow(dpy, 0);

	const auto conf         = XRRGetScreenInfo(dpy, root);
	const auto current_rate = XRRConfigCurrentRate(conf);

	XCloseDisplay(dpy);
	return current_rate;
}
#endif
