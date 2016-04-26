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


using namespace std;
using namespace cpp_nbt;


entity::entity() : x(0), y(0), motion_x(0), motion_y(0) {}
entity::entity(const nbt_compound & from) : entity() {
	read_from_nbt(from);
}
entity::entity(const entity & other) : x(other.x), y(other.y) {}
entity::entity(entity && other) : x(other.x), y(other.y) {}

entity::~entity() {}

void entity::read_from_nbt(const nbt_compound & from) {
	x = *(from.get_float("x") ?: &x);
	y = *(from.get_float("y") ?: &y);
	motion_x = *(from.get_float("motion_x") ?: &motion_x);
	motion_y = *(from.get_float("motion_y") ?: &motion_y);
}

void entity::write_to_nbt(nbt_compound & to) const {
	to.set_float("x", x);
	to.set_float("y", y);
	to.set_float("motion_x", motion_x);
	to.set_float("motion_y", motion_y);
}

void entity::tick(float max_x, float max_y) {
	x += motion_x * .15f * speed();
	y += motion_y * .15f * speed();
	motion_x *= .85f;
	motion_y *= .85f;

	if(max_x && max_y) {
		if(x < 0) {
			x = fabs(x);
			motion_x = -motion_x;
		}
		if(x > max_x) {
			x += max_x - x;
			motion_x = -motion_x;
		}
		if(y < 0) {
			y = fabs(y);
			motion_y = -motion_y;
		}
		if(y > max_y) {
			y += max_y - y;
			motion_y = -motion_y;
		}
	}
}

void entity::start_movement(float amt_x, float amt_y) {
	motion_x += amt_x;
	motion_y += amt_y;
}

float entity::speed() const {
	return 1;
}
