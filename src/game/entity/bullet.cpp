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
#include <seed11/seed11.hpp>


std::unique_ptr<bullet> bullet::create(game_world & world, size_t id, sf::Vector2f aim, unsigned int x, unsigned int y, const bullet_properties & props) {
	auto bull = std::make_unique<bullet>(std::ref(world), id, x, y, std::cref(props));

	aim = normalised(aim);
	// TODO: remove in favour of time-based bullet lifespan
	const auto ignore_up_to = bull->speed_loss() * 10;
	if(std::abs(aim.x) < ignore_up_to)
		aim.x = 0;
	if(std::abs(aim.y) < ignore_up_to)
		aim.y = 0;
	bull->start_movement(aim.x, aim.y);

	return bull;
}

std::unique_ptr<bullet> bullet::create(game_world & world, size_t id, sf::Vector2f aim, unsigned int x, unsigned int y, float spread_min, float spread_max,
                                       const bullet_properties & props) {
	static const auto pi = std::acos(-1.l);
	static auto rand = seed11::make_seeded<std::mt19937>();
	static std::bernoulli_distribution dev_way_dist;

	aim              = normalised(aim);
	double aim_angle = std::atan2(aim.x, aim.y) * 180. / pi;
	auto ang_dev     = std::uniform_real_distribution<double>{spread_min, spread_max}(rand);
	if(dev_way_dist(rand))
		aim_angle += ang_dev;
	else
		aim_angle -= ang_dev;

	auto aim_angle_rad = aim_angle * pi / 180.;
	aim.x              = std::sin(aim_angle_rad);
	aim.y              = std::cos(aim_angle_rad);

	return create(world, id, aim, x, y, props);
}

void bullet::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	static const auto constexpr k = 2.5f;

	const sf::Vertex vertices[]{{{x, y}, sf::Color::White},  //
	                            {{x + motion_x * k, y + motion_y * k}, sf::Color::White}};
	target.draw(vertices, sizeof vertices / sizeof *vertices, sf::PrimitiveType::Lines, states);
}

bullet::bullet(game_world & world_r, size_t id_a, unsigned int px, unsigned int py, const bullet_properties & pprops) : entity(world_r, id_a), props(pprops) {
	x = px;
	y = py;
}

void bullet::read_from_json(const json::object & from) {
	entity::read_from_json(from);
	auto itr = from.end();
	if((itr = from.find("bullet")) != from.end()) {
		const auto bullet = itr->second.as<json::object>();
		props             = {bullet.at("speed").as<float>(), bullet.at("speed_loss").as<float>()};
	}
}

json::object bullet::write_to_json() const {
	auto written = entity::write_to_json();
	written.emplace("bullet", json::object({{"speed", props.speed}, {"speed_loss", props.speed_loss}}));
	written.emplace("kind", "bullet");
	return written;
}

void bullet::tick(float max_x, float max_y) {
	entity::tick(max_x, max_y);

	const auto min_speed = props.speed * props.speed_loss * 10;
	// Only kill the bullet if it's moving in that direction the the first place, see `bullet::create()`
	// TODO: remove in favour of time-based bullet lifespan
	if((motion_x && std::abs(motion_x) < min_speed) && (motion_y && std::abs(motion_y) < min_speed))
		world.despawn(id);
}

float bullet::speed() const {
	return props.speed;
}

float bullet::speed_loss() const {
	return props.speed_loss;
}
