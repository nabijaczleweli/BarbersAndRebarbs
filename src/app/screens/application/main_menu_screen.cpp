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


#include "main_menu_screen.hpp"
#include "../../../reference/container.hpp"
#include "../../../reference/joystick_info.hpp"
#include "../../application.hpp"
#include "../game/main_game_screen.hpp"


using namespace std;
using namespace sf;


typedef main_menu_screen::direction direction;


void main_menu_screen::move_selection(direction dir) {
	switch(dir) {
		case direction::up:
			selected = min(main_buttons.size() - 1, selected + 1);
			break;
		case direction::down:
			if(selected)
				--selected;
			break;
	}
}

void main_menu_screen::press_button() {
	auto itr = main_buttons.begin();
	advance(itr, selected);
	(itr->second)(itr->first);
}

void main_menu_screen::try_drawings() {
	if(!(control_frames_counter++ % 10)) {
		control_frames_counter = 0;
		if(joystick_drawing.first != Joystick::isConnected(0)) {
			joystick_drawing.first ^= 1;
			joystick_drawing.second.move(0, joystick_drawing.second.size().y * .55f * (joystick_drawing.first ? 1 : -1));
			keys_drawing.move(0, joystick_drawing.second.size().y * .55f * (joystick_drawing.first ? -1 : 1));
		}
	}
}

void main_menu_screen::setup() {
	screen::setup();
}

int main_menu_screen::loop() {
	const auto & winsize(app.window.getSize());

	unsigned int buttid = 0;
	for(auto & button : main_buttons) {
		const auto & btnbds(button.first.getGlobalBounds());

		button.first.setPosition((winsize.x * (59.f / 60.f)) - btnbds.width,
		                         (winsize.y * (7.f / 8.f)) - (buttid + 1) * btnbds.height - (winsize.y * ((buttid * 1.f) / 90.f)));
		button.first.setColor((buttid == selected) ? Color::Red : Color::White);
		++buttid;
	}
	return 0;
}

int main_menu_screen::draw() {
	for(const auto & button : main_buttons)
		app.window.draw(button.first);

	try_drawings();
	if(joystick_drawing.first)
		app.window.draw(joystick_drawing.second);
	app.window.draw(keys_drawing);

	return 0;
}

int main_menu_screen::handle_event(const Event & event) {
	if(int i = screen::handle_event(event))
		return i;

	switch(static_cast<int>(event.type)) {
		case Event::MouseMoved: {
			unsigned int buttid = 0;
			for(const auto & button : main_buttons) {
				if(button.first.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
					selected = buttid;
					break;
				}
				++buttid;
			}
		} break;

		case Event::MouseButtonPressed:
			if(event.mouseButton.button == Mouse::Left)
				for(const auto & button : main_buttons)
					if(button.first.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						press_button();
						break;
					}
			break;

		case Event::MouseWheelMoved:
			move_selection((event.mouseWheel.delta > 0) ? direction::up : direction::down);
			break;
		case Event::KeyPressed:
			switch(static_cast<int>(event.key.code)) {
				case Keyboard::Key::Up:
					move_selection(direction::up);
					break;
				case Keyboard::Key::Down:
					move_selection(direction::down);
					break;
				case Keyboard::Key::Return:
				case Keyboard::Key::Space:
					press_button();
					break;
			}
			break;

		case Event::JoystickMoved:
			if(event.joystickMove.axis == X360_axis_mappings::LeftStickVertical || event.joystickMove.axis == X360_axis_mappings::DPadVertical) {
				if(event.joystickMove.position && (event.joystickMove.position >= 25 || event.joystickMove.position <= -25)) {
					if(joystick_up)
						break;
					joystick_up    = true;
					const int sign = event.joystickMove.position / abs(event.joystickMove.position);

					move_selection((sign == ((event.joystickMove.axis == X360_axis_mappings::LeftStickVertical) ? X360_axis_up_right_mappings::LeftStickVertical
					                                                                                            : X360_axis_up_right_mappings::DPadVertical))
					                   ? direction::up
					                   : direction::down);
				} else
					joystick_up = false;
			}
			break;

		case Event::JoystickButtonPressed:
			if(event.joystickButton.button == X360_button_mappings::A)
				press_button();
			break;
	}

	return 0;
}

main_menu_screen::main_menu_screen(application & theapp)
      : screen(theapp), control_frames_counter(0), joystick_up(false), joystick_drawing(false, drawing("xbox", app.window.getSize())),
        keys_drawing("keyboard", app.window.getSize()) {
	keys_drawing.move(app.window.getSize().x / 4 - keys_drawing.size().x / 2, app.window.getSize().y / 2 - keys_drawing.size().y / 2);
	joystick_drawing.second.move(app.window.getSize().x / 4 - joystick_drawing.second.size().x / 2,
	                             app.window.getSize().y / 2 - joystick_drawing.second.size().y / 2);

	main_buttons.emplace_front(Text(global_izer.translate_key("gui.application.text.start"), font_swirly),
	                           [&](Text &) { app.schedule_screen<main_game_screen>(); });
	main_buttons.emplace_front(Text(global_izer.translate_key("gui.application.text."s + (app_configuration.play_sounds ? "" : "un") + "mute"), font_swirly),
	                           [&](Text & txt) {
		                           app_configuration.play_sounds = !app_configuration.play_sounds;
		                           txt.setString(global_izer.translate_key("gui.application.text."s + (app_configuration.play_sounds ? "" : "un") + "mute"));
		                           app.retry_music();
		                         });
	main_buttons.emplace_front(Text(global_izer.translate_key("gui.application.text.quit"), font_swirly), [&](Text &) { app.window.close(); });

	selected = main_buttons.size() - 1;
}
main_menu_screen::main_menu_screen(const main_menu_screen & other)
      : screen(other), main_buttons(other.main_buttons), selected(other.selected), control_frames_counter(0), joystick_up(other.joystick_up),
        joystick_drawing(other.joystick_drawing), keys_drawing(other.keys_drawing) {}
main_menu_screen::main_menu_screen(main_menu_screen && other)
      : screen(move(other)), main_buttons(move(other.main_buttons)), selected(other.selected), control_frames_counter(other.control_frames_counter),
        joystick_up(other.joystick_up), joystick_drawing(move(other.joystick_drawing)), keys_drawing(move(other.keys_drawing)) {}
