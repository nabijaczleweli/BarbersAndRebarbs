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


#include "circle_chunk.hpp"
#include <cmath>
#include <vector>


using namespace sf;
using namespace std;


void circle_chunk::draw(RenderTarget & target, RenderStates states) const {
	static const auto tau = acos(-1) * 2;

	vector<Vertex> vertices;
	vertices.reserve(points);

	for(auto i = 0.; i < points; ++i)
		vertices.emplace_back(Vector2f(r * cos(i / points * fract * tau), r * sin(i / points * fract * tau)), clr);

	states.transform *= getTransform();
	target.draw(vertices.data(), vertices.size(), PrimitiveType::LinesStrip, states);
}

circle_chunk::circle_chunk(float frcn, float rad, unsigned int np) : fract(frcn), r(rad), points(np) {}

float circle_chunk::radius() const {
	return r;
}

void circle_chunk::radius(float new_radius) {
	r = new_radius;
}

float circle_chunk::point_amount() const {
	return points;
}

void circle_chunk::point_amount(float new_point_amount) {
	points = new_point_amount;
}

float circle_chunk::fraction() const {
	return fract;
}

void circle_chunk::fraction(float new_fraction) {
	fract = new_fraction;
}


const Color & circle_chunk::colour() const {
	return clr;
}

void circle_chunk::colour(Color new_colour) {
	clr = std::move(new_colour);
}
