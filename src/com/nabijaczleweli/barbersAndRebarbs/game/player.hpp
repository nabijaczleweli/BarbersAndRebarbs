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


#ifndef PLAYER_HPP
#define PLAYER_HPP


#include "entity.hpp"
#include <SFML/Graphics.hpp>


class player : public entity, public sf::Drawable {
	protected:
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;

	private:

	public:
		player();
		player(const cpp_nbt::nbt_compound & from);
		player(const player & other);
		player(player && other);

		virtual ~player();

		virtual void read_from_nbt(const cpp_nbt::nbt_compound & from) override;
		virtual void write_to_nbt(cpp_nbt::nbt_compound & to) const override;

		virtual void tick(float max_x, float max_y) override;
};


#endif  // PLAYER_HPP
