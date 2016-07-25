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


#include "screen.hpp"
#include "../../reference/container.hpp"
#include "../../reference/joystick_info.hpp"
#include "../application.hpp"
#include <ctime>
#include <iomanip>


void screen::setup() {}

int screen::handle_event(const sf::Event & event) {
	if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) ||
	   (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == X360_button_mappings::Back))
		app.window.close();
	else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5) {
		char str[25];
		const auto tm = time(nullptr);
		strftime(str, sizeof str, "/%Y.%m.%d %H;%M;%S.png", localtime(&tm));
		app.window.capture().saveToFile(screenshots_root + str);
	} else if(event.type == sf::Event::MouseButtonPressed)
		app.window.requestFocus();
	else if(event.type == sf::Event::Count)
		throw sf::Event::Count;
	return 0;
}

screen::screen(application & theapp) : app(theapp) {}
