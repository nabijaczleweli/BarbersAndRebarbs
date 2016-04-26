// The MIT License (MIT)

// Copyright (c) 2015 nabijaczleweli

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
#ifndef SPLASH_SCREEN_HPP
#define SPLASH_SCREEN_HPP


#include "../screen.hpp"
#include "../../../render/managed_sprite.hpp"


class splash_screen : public screen {
	private:
		unsigned int frames = 0;
		managed_sprite background;
		sf::Text text;

	public:
		virtual void setup() override;
		virtual int loop() override;
		virtual int draw() override;
		virtual int handle_event(const sf::Event & event) override;

		splash_screen(application * theapp, unsigned int frame_amt);
		splash_screen(const splash_screen & other);
		splash_screen(splash_screen && other);
		virtual ~splash_screen();
};


#endif  // SPLASH_SCREEN_HPP
