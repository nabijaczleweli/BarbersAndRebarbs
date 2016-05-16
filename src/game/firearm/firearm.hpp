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


#pragma once


#include "../world.hpp"
#include "firearm_properties.hpp"
#include <SFML/System.hpp>
#include <map>
#include <string>


class firearm {
private:
	const firearm_properties * props;
	game_world * world;

public:
	static const std::map<std::string, firearm_properties> & properties();


	firearm(game_world & world, const std::string & gun_id);

	bool trigger(float pos_x, float pos_y, const sf::Vector2f & aim, bool sufficient_stam);
	bool tick(float pos_x, float pos_y, const sf::Vector2f & aim, bool sufficient_stam);
	bool untrigger(float pos_x, float pos_y, const sf::Vector2f & aim, bool sufficient_stam);

	const std::string & id() const noexcept;
	const std::string & name() const noexcept;
};
