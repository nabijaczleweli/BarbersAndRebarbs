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
#include "../entity/bullet.hpp"


using namespace sf;
using namespace std;


firearm::firearm(game_world & w, const string & gun_id)
      : props(&properties().at(gun_id)), world(&w), action_speed(chrono::duration_cast<chrono::high_resolution_clock::duration>(
                                                        chrono::microseconds(static_cast<chrono::microseconds::rep>(props->action_speed * micro::den)))),
        reload_speed(chrono::duration_cast<chrono::high_resolution_clock::duration>(
            chrono::microseconds(static_cast<chrono::microseconds::rep>(props->reload_speed * micro::den)))),
        trigger_pulled(false), left_in_mag(0) {}

bool firearm::trigger(float pos_x, float pos_y, const Vector2f & aim, bool sufficient_stam) {
	const auto now          = chrono::high_resolution_clock::now();
	const auto action_ready = now - action_repeat_start >= action_speed;
	const auto reload_ready = now - mag_reload_start >= reload_speed;
	const auto shoot        = action_ready && reload_ready && left_in_mag && sufficient_stam;

	trigger_pulled = true;

	if(shoot) {
		world->spawn_create<bullet>(aim, pos_x, pos_y, cref(props->bullet_props));
		action_repeat_start = now;
		--left_in_mag;
	}

	return shoot;
}

bool firearm::tick(float pos_x, float pos_y, const Vector2f & aim, bool sufficient_stam) {
	const auto shoot = sufficient_stam && trigger_pulled && left_in_mag && props->fire_mode == firearm_properties::fire_mode_t::full_auto;

	if(shoot) {
		const auto now          = chrono::high_resolution_clock::now();
		const auto action_ready = now - action_repeat_start >= action_speed;
		const auto reload_ready = now - mag_reload_start >= reload_speed;

		if(action_ready && reload_ready) {
			world->spawn_create<bullet>(aim, pos_x, pos_y, cref(props->bullet_props));
			action_repeat_start = now;
			--left_in_mag;
		}

		return action_ready && reload_ready;
	}

	return shoot;
}

bool firearm::untrigger(float pos_x, float pos_y, const Vector2f & aim, bool sufficient_stam) {
	const auto shoot = sufficient_stam && left_in_mag && props->fire_mode == firearm_properties::fire_mode_t::semi_auto_response_trigger;

	trigger_pulled = false;

	if(shoot) {
		const auto now          = chrono::high_resolution_clock::now();
		const auto action_ready = now - action_repeat_start >= action_speed;
		const auto reload_ready = now - mag_reload_start >= reload_speed;

		if(action_ready && reload_ready) {
			world->spawn_create<bullet>(aim, pos_x, pos_y, cref(props->bullet_props));
			action_repeat_start = now;
			--left_in_mag;
		}

		return action_ready && reload_ready;
	}

	return shoot;
}

void firearm::reload() {
	left_in_mag      = props->mag_size;
	mag_reload_start = chrono::high_resolution_clock::now();
}

const string & firearm::id() const noexcept {
	return props->id;
}

const string & firearm::name() const noexcept {
	return props->name;
}
