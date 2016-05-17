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
#include "../../app/application.hpp"
#include "../../reference/container.hpp"
#include "../../reference/joystick_info.hpp"
#include "../world.hpp"
#include "bullet.hpp"
#include "seed11/seed11.hpp"
#include <SFML/Window.hpp>
#include <random>


using namespace cpp_nbt;
using namespace std;
using namespace sf;


pair<bool, Vector2f> player::controller_aim(unsigned int controller_id) const {
	const auto horizontal      = Joystick::getAxisPosition(controller_id, X360_axis_mappings::RightStickHorizontal);
	const auto vertical        = Joystick::getAxisPosition(controller_id, X360_axis_mappings::RightStickVertical);
	const auto out_of_deadzone = abs(horizontal) > app_configuration.controller_deadzone && abs(vertical) > app_configuration.controller_deadzone;

	return {out_of_deadzone, {horizontal, vertical}};
}

void player::draw(RenderTarget & target, RenderStates states) const {
	static const Color body_colour(231, 158, 109);
	static const Color armour_colour(200, 200, 200);

	Vertex vertices[8]{
	    {{x - 1, y}, body_colour},        //
	    {{x, y - 1}, body_colour},        //
	    {{x + 1, y}, body_colour},        //
	    {{x, y + 1}, body_colour},        //
	    {{x - 2, y - 2}, armour_colour},  //
	    {{x + 2, y - 2}, armour_colour},  //
	    {{x + 2, y + 2}, armour_colour},  //
	    {{x - 2, y + 2}, armour_colour},  //
	};
	target.draw(vertices, 8, PrimitiveType::Points, states);
}

player::player(game_world & world_r, size_t id_a, Vector2u screen_size) : entity(world_r, id_a), gun(world_r, "default"), hp(1), fp(1) {
	static auto rand = seed11::make_seeded<mt19937>();

	uniform_real_distribution<float> x_dist(0, screen_size.x - 1);
	uniform_real_distribution<float> y_dist(0, screen_size.y - 1);

	x = x_dist(rand);
	y = y_dist(rand);
}

void player::read_from_nbt(const cpp_nbt::nbt_compound & from) {
	entity::read_from_nbt(from);
	if(auto fgun_id = from.get_string("gun_id"))
		gun = firearm(world, *fgun_id);
	if(auto fhp = from.get_float("hp"))
		hp = *fhp;
	if(auto ffp = from.get_float("fp"))
		fp = *ffp;
}

void player::write_to_nbt(cpp_nbt::nbt_compound & to) const {
	entity::write_to_nbt(to);
	to.set_string("gun_id", gun.id());
	to.set_float("hp", hp);
	to.set_float("fp", fp);
}

void player::tick(float max_x, float max_y) {
	static const auto stamina_regen_frame = app_configuration.player_stamina_regen / application::effective_FPS();

	const auto joystick_connected = Joystick::isConnected(0);

	entity::tick(max_x, max_y);

	auto delta_speed_x = 0.f;
	auto delta_speed_y = 0.f;

	if(Keyboard::isKeyPressed(Keyboard::Key::A))
		delta_speed_x -= 1;
	if(Keyboard::isKeyPressed(Keyboard::Key::D))
		delta_speed_x += 1;
	if(Keyboard::isKeyPressed(Keyboard::Key::W))
		delta_speed_y -= 1;
	if(Keyboard::isKeyPressed(Keyboard::Key::S))
		delta_speed_y += 1;

	if(joystick_connected) {
		const auto horizontal = Joystick::getAxisPosition(0, X360_axis_mappings::LeftStickHorizontal);
		const auto vertical   = Joystick::getAxisPosition(0, X360_axis_mappings::LeftStickVertical);
		if(abs(horizontal) > app_configuration.controller_deadzone && abs(vertical) > app_configuration.controller_deadzone) {
			const auto horizontal_sign = horizontal / abs(horizontal);
			const auto vertical_sign   = vertical / abs(vertical);

			delta_speed_x = ((horizontal_sign == X360_axis_up_right_mappings::RightStickHorizontal) ? abs(horizontal) : -abs(horizontal)) / 100.f;
			delta_speed_y = ((vertical_sign == X360_axis_up_right_mappings::RightStickVertical) ? -abs(vertical) : abs(vertical)) / 100.f;
		}
	}

	start_movement(delta_speed_x, delta_speed_y);

	fp = min(1.f, fp + stamina_regen_frame);

	const auto sufficient_stam = fp >= app_configuration.player_bullet_stamina_cost;

	if(joystick_connected) {
		const auto aim = controller_aim(0);
		if(aim.first)
			if(gun.tick(x, y, aim.second, sufficient_stam))
				fp -= app_configuration.player_bullet_stamina_cost;
	} else if(gun.tick(x, y, static_cast<Vector2f>(Mouse::getPosition()) - Vector2f(x, y), sufficient_stam))
		fp -= app_configuration.player_bullet_stamina_cost;
}

void player::handle_event(const Event & event) {
	const auto sufficient_stam = fp >= app_configuration.player_bullet_stamina_cost;

	if(event.type == Event::EventType::MouseButtonPressed && event.mouseButton.button == Mouse::Button::Left) {
		if(gun.trigger(x, y, static_cast<Vector2f>(Mouse::getPosition()) - Vector2f(x, y), sufficient_stam))
			fp -= app_configuration.player_bullet_stamina_cost;
	} else if(event.type == Event::EventType::JoystickButtonPressed && event.joystickButton.button == X360_button_mappings::RB &&
	          event.joystickButton.joystickId == 0) {
		const auto aim = controller_aim(0);
		if(aim.first)
			if(gun.trigger(x, y, aim.second, sufficient_stam))
				fp -= app_configuration.player_bullet_stamina_cost;
	} else if(event.type == Event::EventType::MouseButtonReleased && event.mouseButton.button == Mouse::Button::Left) {
		if(gun.untrigger(x, y, static_cast<Vector2f>(Mouse::getPosition()) - Vector2f(x, y), sufficient_stam))
			fp -= app_configuration.player_bullet_stamina_cost;
	} else if(event.type == Event::EventType::JoystickButtonReleased && event.joystickButton.button == X360_button_mappings::RB &&
	          event.joystickButton.joystickId == 0) {
		const auto aim = controller_aim(0);
		if(aim.first)
			if(gun.untrigger(x, y, aim.second, sufficient_stam))
				fp -= app_configuration.player_bullet_stamina_cost;
	}
}

float player::speed() const {
	return app_configuration.player_speed;
}

float & player::health() noexcept {
	return hp;
}

const float & player::health() const noexcept {
	return hp;
}

float & player::stamina() noexcept {
	return fp;
}

const float & player::stamina() const noexcept {
	return fp;
}
