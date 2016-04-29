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


#include "player.hpp"
#include "../reference/joystick_info.hpp"
#include <SFML/Window.hpp>


using namespace cpp_nbt;
using namespace std;
using namespace sf;


void player::draw(RenderTarget & target, RenderStates states) const {
	target.draw(&static_cast<const Vertex &>(Vertex(Vector2f(x, y), Color::White)), 1, PrimitiveType::Points, states);
}

player::player() : entity() {
	motion_x = 1000;
	motion_y = 200;
}
player::player(const nbt_compound & from) : entity(from) {}
player::player(const player & other) : entity(other) {}
player::player(player && other) : entity(move(other)) {}

player::~player() {}

void player::read_from_nbt(const cpp_nbt::nbt_compound & from) {
	entity::read_from_nbt(from);
}

void player::write_to_nbt(cpp_nbt::nbt_compound & to) const {
	entity::write_to_nbt(to);
}

void player::tick(float max_x, float max_y) {
	entity::tick(max_x, max_y);

	if(Keyboard::isKeyPressed(Keyboard::Key::Left))
		motion_x -= 1;
	if(Keyboard::isKeyPressed(Keyboard::Key::Right))
		motion_x += 1;
	if(Keyboard::isKeyPressed(Keyboard::Key::Up))
		motion_y -= 1;
	if(Keyboard::isKeyPressed(Keyboard::Key::Down))
		motion_y += 1;
}
