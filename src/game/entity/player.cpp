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
#include "../../util/sound.hpp"
#include "../world.hpp"
#include "bullet.hpp"
#include <SFML/Window.hpp>
#include <random>
#include <seed11/seed11.hpp>


static const sf::Color progress_colour(255, 255, 255, 100);
static const char * gun_pickup_fnames[]{"gear_rattle02.wav", "gear_rattle03.wav", "gear_rattle05.wav"};

static std::vector<audiere::SoundEffectPtr> open_pickup_sounds() {
	std::vector<audiere::SoundEffectPtr> out;
	out.reserve(sizeof gun_pickup_fnames / sizeof *gun_pickup_fnames);
	std::transform(std::begin(gun_pickup_fnames), std::end(gun_pickup_fnames), std::back_inserter(out),
	               [](auto fname) { return audiere::OpenSoundEffect(audio_device, (sound_root + '/' + fname).c_str(), audiere::SoundEffectType::SINGLE); });
	std::for_each(out.begin(), out.end(), [](auto && sound) { sound->setVolume(output_volume(app_configuration.sound_effect_volume)); });
	return out;
}


std::pair<bool, sf::Vector2f> player::controller_aim(unsigned int controller_id) const {
	const auto horizontal      = sf::Joystick::getAxisPosition(controller_id, X360_axis_mappings::RightStickHorizontal);
	const auto vertical        = sf::Joystick::getAxisPosition(controller_id, X360_axis_mappings::RightStickVertical);
	const auto out_of_deadzone = std::abs(horizontal) > app_configuration.controller_deadzone && std::abs(vertical) > app_configuration.controller_deadzone;

	return {out_of_deadzone, {horizontal, vertical}};
}


void player::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	static const sf::Color body_colour(231, 158, 109);
	static const sf::Color armour_colour(200, 200, 200);

	const sf::Vertex vertices[]{
	    {{x - 1, y}, body_colour},        //
	    {{x, y - 1}, body_colour},        //
	    {{x + 1, y}, body_colour},        //
	    {{x, y + 1}, body_colour},        //
	    {{x - 2, y - 2}, armour_colour},  //
	    {{x + 2, y - 2}, armour_colour},  //
	    {{x + 2, y + 2}, armour_colour},  //
	    {{x - 2, y + 2}, armour_colour},  //
	};
	target.draw(vertices, sizeof vertices / sizeof *vertices, sf::PrimitiveType::Points, states);

	const auto gun_progress = gun.progress();
	if(gun_progress != 1) {
		progress_circle.fraction(gun_progress);
		progress_circle.setPosition(x, y);
		progress_circle.setRotation(gun_progress * 360);
		target.draw(progress_circle, states);
	}
	progress = gun.depletion();

	if(gun_name_popup.first) {
		const auto eff_ps             = application::effective_FPS();
		const auto in_out_threshold   = eff_ps / 2;
		const auto fade_in_threshold  = eff_ps * app_configuration.player_gun_popup_length - in_out_threshold;
		const auto fade_out_threshold = in_out_threshold;

		if(gun_name_popup.first >= fade_in_threshold || gun_name_popup.first <= fade_out_threshold) {
			auto colour = gun_name_popup.second.getFillColor();
			if(gun_name_popup.first >= fade_in_threshold)
				colour.a = 255 * (1.f - (static_cast<float>(gun_name_popup.first - fade_in_threshold) / in_out_threshold));
			else if(gun_name_popup.first <= fade_out_threshold)
				colour.a = 255 * (static_cast<float>(gun_name_popup.first) / in_out_threshold);
			gun_name_popup.second.setFillColor(colour);
		}

		const auto & size = gun_name_popup.second.getLocalBounds();
		gun_name_popup.second.setPosition(x - size.width / 2., y - size.height * 2);

		target.draw(gun_name_popup.second);

		if(gun_name_popup.first == fade_in_threshold) {
			if(app_configuration.play_sounds)
				gun_pickup_sounds.second[gun_pickup_sounds.first]->play();
			if(++gun_pickup_sounds.first >= gun_pickup_sounds.second.size())
				gun_pickup_sounds.first = 0;
		}

		--gun_name_popup.first;
	}
}

player::player(game_world & world_r)
      : entity(world_r), progress_circle(0, 7), frames_pressed(0),
        gun_name_popup(application::effective_FPS() * app_configuration.player_gun_popup_length, {"", font_pixelish, 10}),
        gun_pickup_sounds(0, open_pickup_sounds()) {
	progress_circle.colour(progress_colour);
}

player::player(game_world & world_r, size_t id_a, sf::Vector2u screen_size)
      : entity(world_r, id_a), progress_circle(0, 7), gun(world_r, app_configuration.player_default_firearm), hp(1), frames_pressed(0), progress(0),
        gun_name_popup(application::effective_FPS() * app_configuration.player_gun_popup_length, {gun.name(), font_pixelish, 10}),
        gun_pickup_sounds(0, open_pickup_sounds()) {
	static auto rand = seed11::make_seeded<std::mt19937>();

	progress_circle.colour(progress_colour);

	std::uniform_real_distribution<float> x_dist(0, screen_size.x - 1);
	std::uniform_real_distribution<float> y_dist(0, screen_size.y - 1);
	std::uniform_int_distribution<std::size_t> pickup_dist(0, gun_pickup_sounds.second.size() - 1);

	x                       = x_dist(rand);
	y                       = y_dist(rand);
	gun_pickup_sounds.first = pickup_dist(rand);
}

void player::read_from_json(const json::object & from) {
	entity::read_from_json(from);
	auto itr = from.end();
	if((itr = from.find("gun")) != from.end())
		gun = firearm(world, itr->second.as<json::object>());
	if((itr = from.find("hp")) != from.end())
		hp = itr->second.as<float>();

	gun_name_popup.second.setString(gun.name());
}

json::object player::write_to_json() const {
	auto written = entity::write_to_json();
	written.emplace("gun", gun.write_to_json());
	written.emplace("hp", hp);
	written.emplace("kind", "player");
	return written;
}

void player::tick(float max_x, float max_y) {
	const auto joystick_connected = sf::Joystick::isConnected(0);

	entity::tick(max_x, max_y);
	gun.tick(x, y, static_cast<sf::Vector2f>(sf::Mouse::getPosition()) - sf::Vector2f(x, y));

	auto delta_speed_x = 0.f;
	auto delta_speed_y = 0.f;
	auto any_pressed   = false;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		any_pressed = true;
		delta_speed_x -= 1;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		any_pressed = true;
		delta_speed_x += 1;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		any_pressed = true;
		delta_speed_y -= 1;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		any_pressed = true;
		delta_speed_y += 1;
	}

	if(joystick_connected) {
		const auto horizontal = sf::Joystick::getAxisPosition(0, X360_axis_mappings::LeftStickHorizontal);
		const auto vertical   = sf::Joystick::getAxisPosition(0, X360_axis_mappings::LeftStickVertical);
		if(std::abs(horizontal) > app_configuration.controller_deadzone && std::abs(vertical) > app_configuration.controller_deadzone) {
			const auto horizontal_sign = horizontal / std::abs(horizontal);
			const auto vertical_sign   = vertical / std::abs(vertical);

			any_pressed   = true;
			delta_speed_x = ((horizontal_sign == X360_axis_up_right_mappings::RightStickHorizontal) ? std::abs(horizontal) : -std::abs(horizontal)) / 100.f;
			delta_speed_y = ((vertical_sign == X360_axis_up_right_mappings::RightStickVertical) ? -std::abs(vertical) : std::abs(vertical)) / 100.f;
		}
	}

	if(!any_pressed) {
		if(frames_pressed > 4)
			frames_pressed -= 4;
		else if(frames_pressed)
			frames_pressed -= 1;
	} else {
		const auto frames_to_full_speed = app_configuration.player_seconds_to_full_speed * application::effective_FPS();
		const auto accel_scaled         = std::min(frames_pressed / frames_to_full_speed, 1.f);

		start_movement(delta_speed_x * accel_scaled, delta_speed_y * accel_scaled);
		++frames_pressed;
	}
}

void player::handle_event(const sf::Event & event) {
	if(event.type == sf::Event::EventType::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
		gun.trigger(x, y, static_cast<sf::Vector2f>(sf::Mouse::getPosition()) - sf::Vector2f(x, y));
	else if(event.type == sf::Event::EventType::JoystickButtonPressed && event.joystickButton.button == X360_button_mappings::RB &&
	        event.joystickButton.joystickId == 0) {
		const auto aim = controller_aim(0);
		if(aim.first)
			gun.trigger(x, y, aim.second);
	} else if(event.type == sf::Event::EventType::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left)
		gun.untrigger(x, y, static_cast<sf::Vector2f>(sf::Mouse::getPosition()) - sf::Vector2f(x, y));
	else if(event.type == sf::Event::EventType::JoystickButtonReleased && event.joystickButton.button == X360_button_mappings::RB &&
	        event.joystickButton.joystickId == 0) {
		const auto aim = controller_aim(0);
		if(aim.first)
			gun.untrigger(x, y, aim.second);
	} else if((event.type == sf::Event::EventType::KeyPressed && event.key.code == sf::Keyboard::Key::R) ||
	          (event.type == sf::Event::EventType::JoystickButtonReleased && event.joystickButton.button == X360_button_mappings::Y))
		gun.reload();
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

float & player::gun_progress() noexcept {
	return progress;
}

const float & player::gun_progress() const noexcept {
	return progress;
}
