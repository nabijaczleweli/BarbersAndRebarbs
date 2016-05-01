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


#include "splash_screen.hpp"
#include "../../../reference/container.hpp"
#include "../../application.hpp"
#include "main_menu_screen.hpp"


using namespace std;
using namespace sf;


void splash_screen::setup() {
	screen::setup();
	if(!frames)
		app.schedule_screen<main_menu_screen>();

	background.loadFromFile(textures_root + "/gui/main/splash.png");
	const float scale = static_cast<float>(app.window.getSize().y - text.getGlobalBounds().height * 1.5f) / background.getSize().y;
	background.setScale(scale, scale);
	background.setPosition(app.window.getSize().x / 2 - background.getGlobalBounds().width / 2, 0);

	text.setPosition(app.window.getSize().x / 2 - text.getGlobalBounds().width / 2, app.window.getSize().y - text.getGlobalBounds().height * 1.25);
}

int splash_screen::loop() {
	if(!frames)
		app.schedule_screen<main_menu_screen>();
	return 0;
}

int splash_screen::draw() {
	app.window.draw(background);
	app.window.draw(text);

	--frames;
	return 0;
}

int splash_screen::handle_event(const Event & event) {
	if((event.type == Event::KeyPressed && !event.key.alt && !event.key.control && !event.key.shift && !event.key.system) ||
	   event.type == Event::MouseButtonPressed || event.type == Event::JoystickButtonPressed)
		app.schedule_screen<main_menu_screen>();
	else if(event.type == Event::Closed)
		app.window.close();
	return 0;
}

splash_screen::splash_screen(application & theapp, unsigned int frame_amt) : screen(theapp), frames(frame_amt), text(app_name, font_pixelish) {}
splash_screen::splash_screen(const splash_screen & other) : screen(other), frames(other.frames), background(other.background), text(other.text) {}
splash_screen::splash_screen(splash_screen && other) : screen(move(other)), frames(other.frames), background(move(other.background)), text(move(other.text)) {}
