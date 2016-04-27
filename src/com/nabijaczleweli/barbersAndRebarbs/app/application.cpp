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

#include "application.hpp"
#include "../reference/container.hpp"
#include "../resource/localizer.hpp"
#include "../util/broken_gcc.hpp"
#include "../util/configurable.hpp"
#include "../util/file.hpp"
#include "screens/application/splash_screen.hpp"
#include <algorithm>
#include <thread>
#include <SFML/System.hpp>


using namespace sf;
using namespace std;
using namespace cpponfig;


static sequential_music * open_sequential_application_music(bool sound) {
	return (sound ? [&]() {                                                             //  If sounds shall be played
		auto files = list_files(sound_root + "/main_menu");                                 //  list all files
		files.erase(remove_if(files.begin(), files.end(), [&](const string & val) {
			return val.find("music");                                                         //  filter out all that are not prefixed with "music"
		}), files.end());
		files.shrink_to_fit();
		return files.size() ? new sequential_music(files.size(), [&](unsigned int id) {     //  if any matches
			static const auto files_music = files;                                              //  get only ones prefixed with "music"
			return sound_root + "/main_menu/" + files_music[id];
		}) : silent_music.get();                                                            //  else silent
	}() : silent_music.get());                                                          //  else silent
}


static unsigned int actual_fps            = 0;
const unsigned int & application::FPS     = actual_fps;
const unsigned int application::vsync_FPS = 60;


application::application() {}

application::~application() {
	if(current_screen) {
		delete current_screen;
		current_screen = nullptr;
	}
	play_sounds->update_from_boolean();
	play_sounds = nullptr;
}

int application::run() {
	window.create(VideoMode::getDesktopMode(), app_name, Style::Fullscreen);
	if(FPS)
		window.setFramerateLimit(FPS);
	else
		window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);
	mouse_pointer.loadFromFile(textures_root + "/gui/general/cursor.png");


	Image * icon = new Image;
	if(icon->loadFromFile(textures_root + "/gui/general/window_main.png"))
		window.setIcon(icon->getSize().x, icon->getSize().x, icon->getPixelsPtr());
	delete icon;
	icon = nullptr;

	schedule_screen<splash_screen>(splash_length * effective_FPS());
	return loop();
}

int application::loop() {
	retry_music();

	while(window.isOpen()) {
		while(temp_screen) {
			delete current_screen;
			current_screen = temp_screen;
			current_screen->setup();
			temp_screen = nullptr;
		}

		if(const int i = current_screen->loop())
			return i;
		music->tick();

		if(const int i = draw())
			return i;

		Event event;
		while(window.pollEvent(event))
			if(const int i = current_screen->handle_event(event))
				return i;
	}
	return 0;
}

int application::draw() {
	window.clear(Color::Black);

	if(const int i = current_screen->draw())
		return i;

	mouse_pointer.setPosition(static_cast<Vector2f>(Mouse::getPosition(window)));
	window.draw(mouse_pointer);

	window.display();
	return 0;
}

void application::retry_music() {
	music = open_sequential_application_music(play_sounds->boolean());
	music->play();
}

void application::config(configuration & cfg) {
	actual_fps = cfg.get("application:FPS", property("0", "0 -> VSYNC; Not 0 -> value (60 HIGHLY recommended)")).unsigned_integer();

	play_sounds   = &cfg.get("application:play_sounds", "true");
	splash_length = cfg.get("application:splash_length", property("2", "Seconds")).unsigned_integer();
}
