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


#include "../../../render/drawing.hpp"
#include "../screen.hpp"
#include <audiere.h>
#include <cpr/cpr.h>
#include <functional>
#include <list>
#include <utility>


class main_menu_screen : public screen {
public:
	enum class direction : unsigned char { up, down };
	using button_clickable = std::pair<sf::Text, std::function<void(sf::Text &)>>;

private:
	std::list<button_clickable> main_buttons;
	std::size_t selected, control_frames_counter;
	bool joystick_up;
	std::pair<bool, drawing> joystick_drawing;
	drawing keys_drawing;
	std::tuple<std::future<cpr::Response>, std::thread, sf::Text, bool> update;
	audiere::SoundEffectPtr selected_option_switch_sound;

	void move_selection(direction dir);
	void press_button();
	void try_drawings();
	void load_game(sf::Text & txt, const std::string & save_path);
	void set_default_menu_items();
	void set_config_menu_items();

public:
	virtual void setup() override;
	virtual int loop() override;
	virtual int draw() override;
	virtual int handle_event(const sf::Event & event) override;

	main_menu_screen(application & theapp);
	virtual ~main_menu_screen();
};
