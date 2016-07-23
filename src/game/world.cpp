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
#include "../reference/container.hpp"
#include "entity/event_handler.hpp"
#include "entity/player.hpp"
#include <fstream>
#include <jsonpp/parser.hpp>
#include <random>
#include <seed11/seed11.hpp>


static const constexpr auto max_id_len       = 4u;
static const constexpr auto max_random_tries = 10u;


size_t game_world::reserve_eid() {
	static auto rand    = seed11::make_seeded<std::mt19937>();
	static auto id_dist = std::uniform_int_distribution<size_t>(0, pow(10, max_id_len));

	for(auto i = 0u; i < max_random_tries; ++i) {
		const auto generated = id_dist(rand);
		if(entities.find(generated) == entities.end())
			return generated;
	}

	return std::uniform_int_distribution<size_t>{}(rand);
}

size_t game_world::spawn_p(size_t id, std::unique_ptr<entity> ent) {
	entities.emplace(id, move(ent));
	return id;
}

entity & game_world::ent(size_t id) {
	return *entities.at(id);
}

const entity & game_world::ent(size_t id) const {
	return *entities.at(id);
}

void game_world::tick(sf::Vector2u screen_size) {
	ticking = true;
	for(auto && entity : entities)
		entity.second->tick(screen_size.x, screen_size.y);
	ticking = false;

	for(auto id : sheduled_for_deletion)
		entities.erase(id);
	sheduled_for_deletion.clear();
}

void game_world::handle_event(const sf::Event & event) {
	if(event.type == sf::Event::EventType::KeyPressed && event.key.control && event.key.code == sf::Keyboard::Key::LBracket) {
		json::object ents;
		for(auto && pr : entities)
			ents.emplace(std::to_string(pr.first), pr.second->write_to_json());

		std::ofstream out(app_configuration.path + ".sav");
		json::dump(out, ents, {0, json::format_options::minify, 20});
	}

	for(const auto & entity : entities)
		if(auto handler = dynamic_cast<event_handler *>(entity.second.get()))
			handler->handle_event(event);
}

void game_world::draw(sf::RenderTarget & upon) {
	for(const auto & entity : entities)
		if(const auto drwbl = dynamic_cast<const sf::Drawable *>(entity.second.get()))
			upon.draw(*drwbl);
}

void game_world::despawn(size_t id) {
	if(ticking)
		sheduled_for_deletion.emplace_back(id);
	else
		entities.erase(id);
}
