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


#include "entity.hpp"
#include <cmath>
#include <functional>


entity::entity(game_world & world_r, size_t id_a) : x(0), y(0), motion_x(0), motion_y(0), id(id_a), world(world_r) {}
entity::entity(game_world & world_r, size_t id_a, const json::object & from) : entity(world_r, id_a) {
	read_from_json(from);
}

void entity::read_from_json(const json::object & from) {
	auto itr = from.end();
	if((itr = from.find("x")) == from.end())
		x = itr->second.as<float>();
	if((itr = from.find("y")) == from.end())
		y = itr->second.as<float>();
	if((itr = from.find("motion_x")) == from.end())
		motion_x = itr->second.as<float>();
	if((itr = from.find("motion_y")) == from.end())
		motion_y = itr->second.as<float>();
}

json::object entity::write_to_json() const {
	return {
	    {"x", x},  //
	    {"y", y},
	    {"motion_x", motion_x},
	    {"motion_y", motion_y},
	};
}

void entity::tick(float max_x, float max_y) {
	const auto resisted_speed = 1 - speed_loss();
	x += motion_x * .15 * speed();
	y += motion_y * .15 * speed();
	motion_x *= resisted_speed;
	motion_y *= resisted_speed;

	if(max_x && max_y) {
		if(x < 0) {
			x        = std::abs(x);
			motion_x = -motion_x;
		}
		if(x > max_x) {
			x += max_x - x;
			motion_x = -motion_x;
		}
		if(y < 0) {
			y        = std::abs(y);
			motion_y = -motion_y;
		}
		if(y > max_y) {
			y += max_y - y;
			motion_y = -motion_y;
		}
	}
}

void entity::start_movement(float amt_x, float amt_y) {
	const auto spd = speed();
	motion_x += amt_x * spd;
	motion_y += amt_y * spd;
}

float entity::speed() const {
	return 1;
}

float entity::speed_loss() const {
	return .15f;
}
