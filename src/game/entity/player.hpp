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


#pragma once


#include "../../render/circle_chunk.hpp"
#include "../firearm/firearm.hpp"
#include "entity.hpp"
#include "event_handler.hpp"
#include <SFML/Graphics.hpp>
#include <audiere.h>


class player : public entity, public event_handler, public sf::Drawable {
protected:
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

	std::pair<bool, sf::Vector2f> controller_aim(unsigned int controller_id) const;
	void create_gun_name_popup();

private:
	mutable circle_chunk progress_circle;
	firearm gun;
	float hp;
	std::size_t frames_pressed;
	mutable float progress;
	mutable std::pair<unsigned int, sf::Text> gun_name_popup;
	mutable std::pair<std::size_t, std::vector<audiere::SoundEffectPtr>> gun_pickup_sounds;

public:
	player(game_world & world);
	player(game_world & world, std::size_t id, sf::Vector2u screen_size);

	virtual ~player() = default;

	virtual void read_from_json(const json::object & from) override;
	virtual json::object write_to_json() const override;

	virtual void tick(float max_x, float max_y) override;
	virtual void handle_event(const sf::Event & event) override;

	virtual float speed() const override;

	float & health() noexcept;
	const float & health() const noexcept;

	float & gun_progress() noexcept;
	const float & gun_progress() const noexcept;
};
