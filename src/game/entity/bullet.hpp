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


#include "../firearm/firearm_properties.hpp"
#include "entity.hpp"
#include <SFML/Graphics.hpp>


class bullet : public entity, public sf::Drawable {
protected:
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
	const bullet_properties & props;

public:
	static std::unique_ptr<bullet> create(game_world & world, std::size_t id, sf::Vector2f aim, unsigned int x, unsigned int y, const bullet_properties & props);


	using entity::entity;
	bullet(game_world & world, std::size_t id, unsigned int x, unsigned int y, const bullet_properties & props);

	virtual ~bullet() = default;


	virtual void tick(float max_x = 0, float max_y = 0) override;

	virtual float speed() const override;
	virtual float speed_loss() const override;
};
