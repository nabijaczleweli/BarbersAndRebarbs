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
#include "../../reference/container.hpp"
#include "../../reference/joystick_info.hpp"
#include "../../util/vector.hpp"
#include "../world.hpp"
#include <SFML/Window.hpp>
#include <cmath>


using namespace cpp_nbt;
using namespace std;
using namespace sf;


unique_ptr<bullet> bullet::create(game_world & world, size_t id, Vector2f aim, unsigned int x, unsigned int y, const bullet_properties & props) {
	auto bull = make_unique<bullet>(ref(world), id, x, y, cref(props));

	aim = normalised(aim);
	// TODO: remove in favour of time-based bullet lifespan
	const auto ignore_up_to = bull->speed_loss() * 10;
	if(abs(aim.x) < ignore_up_to)
		aim.x = 0;
	if(abs(aim.y) < ignore_up_to)
		aim.y = 0;
	bull->start_movement(aim.x, aim.y);

	return bull;
}

void bullet::draw(RenderTarget & target, RenderStates states) const {
	static const auto constexpr k = 2.5f;

	const Vertex vertices[]{{{x, y}, Color::White},  //
	                        {{x + motion_x * k, y + motion_y * k}, Color::White}};
	target.draw(vertices, sizeof vertices / sizeof *vertices, PrimitiveType::Lines, states);
}

bullet::bullet(game_world & world_r, size_t id_a, unsigned int px, unsigned int py, const bullet_properties & pprops) : entity(world_r, id_a), props(pprops) {
	x = px;
	y = py;
}

void bullet::tick(float max_x, float max_y) {
	entity::tick(max_x, max_y);

	const auto min_speed = props.speed * props.speed_loss * 10;
	// Only kill the bullet if it's moving in that direction the the first place, see `bullet::create()`
	// TODO: remove in favour of time-based bullet lifespan
	if((motion_x && abs(motion_x) < min_speed) && (motion_y && abs(motion_y) < min_speed))
		world.despawn(id);
}

float bullet::speed() const {
	return props.speed;
}

float bullet::speed_loss() const {
	return props.speed_loss;
}
