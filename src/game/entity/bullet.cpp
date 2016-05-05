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


#include "bullet.hpp"
#include "../../reference/joystick_info.hpp"
#include "../../reference/container.hpp"
#include "../../util/vector.hpp"
#include <SFML/Window.hpp>


using namespace cpp_nbt;
using namespace std;
using namespace sf;


unique_ptr<bullet> bullet::create(game_world & world, size_t id, Vector2f aim, unsigned int x, unsigned int y) {
	auto bull = make_unique<bullet>(ref(world), id, x, y);

	aim = normalised(aim);
	bull->start_movement(aim.x, aim.y);

	return bull;
}

void bullet::draw(RenderTarget & target, RenderStates states) const {
	CircleShape crcl(5);
	crcl.setPosition(x - 2.5, y - 2.5);
	target.draw(crcl, states);
}

bullet::bullet(game_world & world_r, size_t id_a, unsigned int px, unsigned int py) : entity(world_r, id_a) {
	x = px;
	y = py;
}

float bullet::speed() const {
	return 5.5;
}

float bullet::speed_loss() const {
	return .001;
}
