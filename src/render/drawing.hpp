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


#include "bezier_curve.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>


class drawing : public sf::Drawable {
public:
	using line     = std::array<sf::Vertex, 2>;
	using triangle = std::array<sf::Vertex, 3>;

private:
	sf::Vector2<double> loaded_size;
	std::vector<line> lines;
	std::vector<triangle> triangles;
	std::vector<bezier_curve> curves;

public:
	sf::Vector2f own_scale;

	drawing(const std::string & model_name, const sf::Vector2f & start);

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

	void move(float x, float y);
	void scale_size(sf::Vector2f factor);
	sf::Vector2f size() const;
};
