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


#include "firearm.hpp"
#include "../../util/json.hpp"
#include "../entity/bullet.hpp"


using namespace std::literals;


firearm::firearm() : props(nullptr), world(nullptr) {}

firearm::firearm(game_world & w, const std::string & gun_id)
      : props(&properties().at(gun_id)), world(&w),
        action_speed(std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
            std::chrono::microseconds(static_cast<std::chrono::microseconds::rep>(props->action_speed * std::micro::den)))),
        reload_speed(std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
            std::chrono::microseconds(static_cast<std::chrono::microseconds::rep>(props->reload_speed * std::micro::den)))),
        trigger_pulled(false), left_in_mag(0), left_mags(props->mag_quantity) {}

firearm::firearm(game_world & w, const json::object & from) : firearm(w, json_get_defaulted(from, "id", "default"s)) {
	read_from_json(from);
}

void firearm::read_from_json(const json::object & from) {
	auto itr = from.end();
	if((itr = from.find("trigger_pulled")) != from.end())
		trigger_pulled = itr->second.as<bool>();
	if((itr = from.find("left_in_mag")) != from.end())
		left_in_mag = itr->second.as<unsigned int>();
	if((itr = from.find("left_mags")) != from.end())
		left_mags = itr->second.as<unsigned int>();
}

json::object firearm::write_to_json() const {
	return {
	    {"trigger_pulled", trigger_pulled},  //
	    {"left_in_mag", left_in_mag}, //
	    {"left_mags", left_mags}, //
	};
}

void firearm::trigger(float pos_x, float pos_y, const sf::Vector2f & aim) {
	const auto now          = std::chrono::high_resolution_clock::now();
	const auto action_ready = now - action_repeat_start >= action_speed;
	const auto reload_ready = now - mag_reload_start >= reload_speed;
	const auto shoot        = action_ready && reload_ready && left_in_mag;

	trigger_pulled = true;

	if(shoot) {
		world->spawn_create<bullet>(aim, pos_x, pos_y, std::cref(props->bullet_props));
		action_repeat_start = now;
		--left_in_mag;
	}
}

void firearm::tick(float pos_x, float pos_y, const sf::Vector2f & aim) {
	const auto shoot = trigger_pulled && left_in_mag && props->fire_mode == firearm_properties::fire_mode_t::full_auto;

	if(shoot) {
		const auto now          = std::chrono::high_resolution_clock::now();
		const auto action_ready = now - action_repeat_start >= action_speed;
		const auto reload_ready = now - mag_reload_start >= reload_speed;

		if(action_ready && reload_ready) {
			world->spawn_create<bullet>(aim, pos_x, pos_y, std::cref(props->bullet_props));
			action_repeat_start = now;
			--left_in_mag;
		}
	}
}

void firearm::untrigger(float pos_x, float pos_y, const sf::Vector2f & aim) {
	const auto shoot = left_in_mag && props->fire_mode == firearm_properties::fire_mode_t::semi_auto_response_trigger;

	trigger_pulled = false;

	if(shoot) {
		const auto now          = std::chrono::high_resolution_clock::now();
		const auto action_ready = now - action_repeat_start >= action_speed;
		const auto reload_ready = now - mag_reload_start >= reload_speed;

		if(action_ready && reload_ready) {
			world->spawn_create<bullet>(aim, pos_x, pos_y, std::cref(props->bullet_props));
			action_repeat_start = now;
			--left_in_mag;
		}
	}
}

void firearm::reload() {
	if(left_mags) {
		left_in_mag      = props->mag_size;
		mag_reload_start = std::chrono::high_resolution_clock::now();
		--left_mags;
	} else
		left_in_mag = 0;
}

const std::string & firearm::id() const noexcept {
	return props->id;
}

const std::string & firearm::name() const noexcept {
	return props->name;
}

float firearm::progress() const noexcept {
	const auto now             = std::chrono::high_resolution_clock::now();
	const auto reload_progress = (now - mag_reload_start).count() / static_cast<double>(reload_speed.count());

	if(reload_progress >= 1) {
		const auto action_progress = (now - action_repeat_start).count() / static_cast<double>(action_speed.count());
		return std::min(action_progress, 1.);
	} else
		return std::min(reload_progress, 1.);
}

float firearm::depletion() const noexcept {
	const auto now          = std::chrono::high_resolution_clock::now();
	const auto reload_ready = now - mag_reload_start >= reload_speed;
	if(reload_ready)
		return left_in_mag / static_cast<float>(props->mag_size);
	else
		return left_mags / static_cast<float>(props->mag_quantity);
}
