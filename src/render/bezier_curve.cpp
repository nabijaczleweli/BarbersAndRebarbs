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


#include "bezier_curve.hpp"
#include "../util/vector.hpp"
#include <algorithm>
#include <functional>


float bezier_curve::bezier_step       = .0001f;
bool bezier_curve::optimize_points    = true;
float bezier_curve::optimize_prettily = .01f;


bezier_curve::bezier_curve(sf::Vector2f the_start_point, sf::Vector2f the_control_point, sf::Vector2f the_end_point)
      : start_point(the_start_point), control_point(the_control_point), end_point(the_end_point) {
	compute_vertices();
}

void bezier_curve::swap(bezier_curve & with) {
	std::swap(start_point, with.start_point);
	std::swap(control_point, with.control_point);
	std::swap(end_point, with.end_point);
	vertices.swap(with.vertices);
}

void bezier_curve::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(vertices.data(), vertices.size(), sf::PrimitiveType::LineStrip, states);
}

void bezier_curve::compute_vertices() {
	vertices.clear();
	for(float t = 0; t <= 1; t += bezier_step)
		vertices.emplace_back((1 - t) * (1 - t) * start_point + 2 * (1 - t) * t * control_point + t * t * end_point, sf::Color::White);

	if(optimize_points) {
		const unsigned int additional_offset = optimize_prettily * vertices.size();
		vertices.erase(
		    unique(vertices.begin() + additional_offset, vertices.end() - additional_offset,
		           [&](auto && lhs, auto && rhs) { return (round(lhs.position.x) == round(rhs.position.x)) && (round(lhs.position.y) == round(rhs.position.y)); }),
		    vertices.end() - additional_offset);
	}
	vertices.shrink_to_fit();
}
