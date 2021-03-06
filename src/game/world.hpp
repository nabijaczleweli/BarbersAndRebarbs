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


#pragma once


#include "entity/entity.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <functional>
#include <jsonpp/value.hpp>
#include <map>
#include <thread>
#include <vector>


class game_world {
private:
	std::map<std::size_t, std::unique_ptr<entity>> entities;
	std::vector<std::size_t> sheduled_for_deletion;
	bool ticking = false;
	std::pair<sf::Text, unsigned int> save_text;
	std::pair<sf::Text, unsigned int> save_error_text;
	std::vector<std::thread> save_threads;

	std::size_t reserve_eid();
	std::size_t spawn_p(std::size_t id, std::unique_ptr<entity> ep);

public:
	entity & ent(std::size_t id);
	const entity & ent(std::size_t id) const;

	void tick(sf::Vector2u screen_size);
	void handle_event(const sf::Event & event);
	void draw(sf::RenderTarget & upon);

	template <class ET, class... AT>
	std::size_t spawn(AT &&... at) {
		const auto id = reserve_eid();
		return spawn_p(id, std::make_unique<ET>(std::ref(*this), id, std::forward<AT>(at)...));
	}

	template <class ET, class... AT>
	std::size_t spawn_create(AT &&... at) {
		const auto id = reserve_eid();
		return spawn_p(id, ET::create(*this, id, std::forward<AT>(at)...));
	}

	void despawn(std::size_t id);

	game_world() = default;
	game_world(const json::object & save, std::size_t & pid);
	~game_world();
};
