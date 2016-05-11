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


#include "world.hpp"
#include "entity/event_handler.hpp"
#include "entity/player.hpp"
#include "seed11/seed11.hpp"
#include <random>


static const constexpr auto max_id_len       = 4u;
static const constexpr auto max_random_tries = 10u;


using namespace sf;
using namespace std;


size_t game_world::reserve_eid() {
	static auto rand    = seed11::make_seeded<mt19937>();
	static auto id_dist = uniform_int_distribution<size_t>(0, pow(10, max_id_len));

	for(auto i = 0u; i < max_random_tries; ++i) {
		const auto generated = id_dist(rand);
		if(entities.find(generated) == entities.end())
			return generated;
	}

	return uniform_int_distribution<size_t>{}(rand);
}

size_t game_world::spawn_p(size_t id, unique_ptr<entity> ent) {
	entities.emplace(id, move(ent));
	return id;
}

entity & game_world::ent(size_t id) {
	return *entities.at(id);
}

const entity & game_world::ent(size_t id) const {
	return *entities.at(id);
}

void game_world::tick(Vector2u screen_size) {
	for(auto && id : sheduled_for_deletion)
		entities.erase(id);

	for(auto && entity : entities)
		entity.second->tick(screen_size.x, screen_size.y);
}

void game_world::handle_event(const Event & event) {
	for(const auto & entity : entities)
		if(auto handler = dynamic_cast<event_handler *>(entity.second.get()))
			handler->handle_event(event);
}

void game_world::draw(RenderTarget & upon) {
	for(const auto & entity : entities)
		if(const auto drwbl = dynamic_cast<const Drawable *>(entity.second.get()))
			upon.draw(*drwbl);
}

void game_world::despawn(size_t id) {
	sheduled_for_deletion.emplace_back(id);
}
