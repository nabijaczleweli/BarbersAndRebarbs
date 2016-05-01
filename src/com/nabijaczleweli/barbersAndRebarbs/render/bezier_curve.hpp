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


#ifndef BEZIER_CURVE_HPP
#define BEZIER_CURVE_HPP


#include <SFML/Graphics.hpp>
#include <future>
#include <vector>


// Quadratic Bezier Curve
//
// t = 0.5; // given example value
// x = (1 - t) * (1 - t) * p[0].x + 2 * (1 - t) * t * p[1].x + t * t * p[2].x;
// y = (1 - t) * (1 - t) * p[0].y + 2 * (1 - t) * t * p[1].y + t * t * p[2].y;
//
// t -> % of curve
// p[0] -> start
// p[1] -> control
// p[2] -> end


class bezier_curve : public sf::Drawable {
private:
	std::vector<sf::Vertex> vertices;
	sf::Transformable transform;

public:
	static float bezier_step;
	static bool optimize_points;
	static float optimize_prettily;


	sf::Vector2f start_point;
	sf::Vector2f control_point;
	sf::Vector2f end_point;

	bezier_curve(sf::Vector2f the_start_point, sf::Vector2f the_control_point, sf::Vector2f the_end_point);
	bezier_curve(const bezier_curve & other);
	bezier_curve(bezier_curve && other);

	~bezier_curve();

	bezier_curve & operator=(const bezier_curve & from);
	void swap(bezier_curve & with);

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

	void compute_vertices();  // Call after messing with `*_point`s


	void move(float x, float y);
	void scale(const sf::Vector2f & factor);
	void scale(float factor_x, float factor_y);
	void set_scale(float factor_x, float factor_y);
	inline const sf::Vector2f & get_scale() const { return transform.getScale(); }
};


#endif  // BEZIER_CURVE_HPP
