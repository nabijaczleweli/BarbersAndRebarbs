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
#ifndef MAIN_GAME_SCREEN_HPP
#define MAIN_GAME_SCREEN_HPP


#include "../screen.hpp"
#include "../../../game/entity.hpp"
#include "../../../game/player.hpp"
#include "../../../render/managed_sprite.hpp"
#include "../../../render/stat_bar.hpp"
#include <deque>
#include <memory>


class main_game_screen : public screen {
private:
	stat_bar hp_stat, energy_stat;
	player the_player;
	std::deque<std::unique_ptr<entity>> entities;

public:
	virtual void setup() override;
	virtual int loop() override;
	virtual int draw() override;
	virtual int handle_event(const sf::Event & event) override;

	main_game_screen(application & theapp);
	main_game_screen(const main_game_screen & other);
	main_game_screen(main_game_screen && other);
	virtual ~main_game_screen() = default;
};


#endif  // MAIN_GAME_SCREEN_HPP
