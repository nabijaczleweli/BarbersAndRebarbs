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
#include <functional>
#include <algorithm>
#include <future>


using namespace std;
using namespace sf;


float bezier_curve::bezier_step       = .0001f;
bool bezier_curve::optimize_points    = true;
float bezier_curve::optimize_prettily = .01f;


bezier_curve::bezier_curve(Vector2f the_start_point, Vector2f the_control_point, Vector2f the_end_point)
      : start_point(the_start_point), control_point(the_control_point), end_point(the_end_point) {
	compute_vertices();
}
bezier_curve::bezier_curve(const bezier_curve & other)
      : vertices(other.vertices), start_point(other.start_point), control_point(other.control_point), end_point(other.end_point) {}
bezier_curve::bezier_curve(bezier_curve && other)
      : vertices(std::move(other.vertices)), start_point(std::move(other.start_point)), control_point(std::move(other.control_point)),
        end_point(std::move(other.end_point)) {}

bezier_curve::~bezier_curve() {}

bezier_curve & bezier_curve::operator=(const bezier_curve & from) {
	bezier_curve temp(from);
	swap(temp);
	return *this;
}

void bezier_curve::swap(bezier_curve & with) {
#define VSWAP(var)       \
	{                      \
		temp     = var;      \
		var      = with.var; \
		with.var = temp;     \
	}

	Vector2f temp;

	VSWAP(start_point);
	VSWAP(control_point);
	VSWAP(end_point);
	vertices.swap(with.vertices);

#undef VSWAP
}

void bezier_curve::draw(RenderTarget & target, RenderStates) const {
	target.draw(vertices.data(), vertices.size(), PrimitiveType::LinesStrip);
	// target.draw(&static_cast<const Vertex &>(Vertex(control_point, Color::White)), 1, PrimitiveType::Points);  // Debug
}

void bezier_curve::compute_vertices() {
	vertices.clear();
	const auto & scale(get_scale());
	for(float t = 0; t <= 1; t += bezier_step)
		vertices.emplace_back((1 - t) * (1 - t) * (scale * start_point) + 2 * (1 - t) * t * (scale * control_point) + t * t * (scale * end_point), Color::White);
	if(optimize_points) {
		const unsigned int additional_offset = optimize_prettily * vertices.size();
		vertices.erase(unique(vertices.begin() + additional_offset, vertices.end() - additional_offset,
		                      [&](const Vertex & lhs, const Vertex & rhs) {
			                      return (round(lhs.position.x) == round(rhs.position.x)) && (round(lhs.position.y) == round(rhs.position.y));
			                    }),
		               vertices.end() - additional_offset);
	}
	vertices.shrink_to_fit();
}

void bezier_curve::move(float x, float y) {
	Vector2f by(x, y);
	start_point += by;
	control_point += by;
	end_point += by;
	compute_vertices();
}

void bezier_curve::set_scale(float factor_x, float factor_y) {
	transform.setScale(factor_x, factor_y);
	compute_vertices();
}

void bezier_curve::scale(const Vector2f & factor) {
	scale(factor.x, factor.y);
}

void bezier_curve::scale(float factor_x, float factor_y) {
	transform.scale(factor_x, factor_y);
	compute_vertices();
}
