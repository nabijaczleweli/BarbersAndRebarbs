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


#include <SFML/Graphics.hpp>


class circle_chunk : public sf::Drawable, public sf::Transformable {
private:
	float fract;
	float r;
	float points;
	sf::Color clr;
	mutable bool recompute_next_time;
	mutable std::vector<sf::Vertex> vertices;

protected:
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

public:
	circle_chunk(float fraction = 1, float radius = 0, unsigned int point_amount = 30);

	float radius() const;
	void radius(float new_radius);

	float point_amount() const;
	void point_amount(float new_point_amount);

	float fraction() const;
	void fraction(float new_fraction);

	const sf::Color & colour() const;
	void colour(sf::Color new_colour);
};
