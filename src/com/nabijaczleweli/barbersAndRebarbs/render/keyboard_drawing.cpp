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


#include "keyboard_drawing.hpp"


using namespace std;
using namespace sf;


template <class T>
inline Vector2<T> operator*(const Vector2<T> & lhs, const Vector2<T> & rhs) {
	Vector2<T> temp(lhs);
	temp.x *= rhs.x;
	temp.y *= rhs.y;
	return temp;
}


// WIDTH = border.x
// HEIGHT = border.y
keyboard_drawing::keyboard_drawing(const Vector2f & s)
      : own_scale(1, 1), border_top({Vector2f(0, 0) + s}, {Vector2f(190, 0) + s}), border_right({Vector2f(190, 0) + s}, {Vector2f(190, 130) + s}),
        border_bottom({Vector2f(190, 130) + s}, {Vector2f(0, 130) + s}), border_left({Vector2f(0, 130) + s}, {Vector2f(0, 0) + s}),
        up_arrow_top({Vector2f(70, 10) + s}, {Vector2f(120, 10) + s}), up_arrow_right({Vector2f(120, 10) + s}, {Vector2f(120, 60) + s}),
        up_arrow_bottom({Vector2f(120, 60) + s}, {Vector2f(70, 60) + s}), up_arrow_left({Vector2f(70, 60) + s}, {Vector2f(70, 10) + s}),
        left_arrow_top({Vector2f(10, 70) + s}, {Vector2f(60, 70) + s}), left_arrow_right({Vector2f(60, 70) + s}, {Vector2f(60, 120) + s}),
        left_arrow_bottom({Vector2f(60, 120) + s}, {Vector2f(10, 120) + s}), left_arrow_left({Vector2f(10, 120) + s}, {Vector2f(10, 70) + s}),
        down_arrow_top({Vector2f(70, 70) + s}, {Vector2f(120, 70) + s}), down_arrow_right({Vector2f(120, 70) + s}, {Vector2f(120, 120) + s}),
        down_arrow_bottom({Vector2f(120, 120) + s}, {Vector2f(70, 120) + s}), down_arrow_left({Vector2f(70, 120) + s}, {Vector2f(70, 70) + s}),
        right_arrow_top({Vector2f(130, 70) + s}, {Vector2f(180, 70) + s}), right_arrow_right({Vector2f(180, 70) + s}, {Vector2f(180, 120) + s}),
        right_arrow_bottom({Vector2f(180, 120) + s}, {Vector2f(130, 120) + s}), right_arrow_left({Vector2f(130, 120) + s}, {Vector2f(130, 70) + s}),
        // Tip -> clockwise
        up_arrow_arrow({Vector2f(95, 27) + s}, {Vector2f(103, 43) + s}, {Vector2f(87, 43) + s}),
        left_arrow_arrow({Vector2f(27, 95) + s}, {Vector2f(43, 103) + s}, {Vector2f(43, 87) + s}),
        down_arrow_arrow({Vector2f(95, 103) + s}, {Vector2f(103, 87) + s}, {Vector2f(87, 87) + s}),
        right_arrow_arrow({Vector2f(163, 95) + s}, {Vector2f(147, 103) + s}, {Vector2f(147, 87) + s}) {}
keyboard_drawing::keyboard_drawing(const Vector2f & start, const Vector2f & size) : keyboard_drawing(start) {
	scale_size(size);
}
#define CPY(field) field(other.field)
#define MOV(field) field(std::move(other.field))
keyboard_drawing::keyboard_drawing(const keyboard_drawing & other)
      : CPY(own_scale), CPY(border_top), CPY(border_right), CPY(border_bottom), CPY(border_left), CPY(up_arrow_top), CPY(up_arrow_right), CPY(up_arrow_bottom),
        CPY(up_arrow_left), CPY(left_arrow_top), CPY(left_arrow_right), CPY(left_arrow_bottom), CPY(left_arrow_left), CPY(down_arrow_top),
        CPY(down_arrow_right), CPY(down_arrow_bottom), CPY(down_arrow_left), CPY(right_arrow_top), CPY(right_arrow_right), CPY(right_arrow_bottom),
        CPY(right_arrow_left), CPY(up_arrow_arrow), CPY(left_arrow_arrow), CPY(down_arrow_arrow), CPY(right_arrow_arrow) {}
keyboard_drawing::keyboard_drawing(keyboard_drawing && other)
      : MOV(own_scale), MOV(border_top), MOV(border_right), MOV(border_bottom), MOV(border_left), MOV(up_arrow_top), MOV(up_arrow_right), MOV(up_arrow_bottom),
        MOV(up_arrow_left), MOV(left_arrow_top), MOV(left_arrow_right), MOV(left_arrow_bottom), MOV(left_arrow_left), MOV(down_arrow_top),
        MOV(down_arrow_right), MOV(down_arrow_bottom), MOV(down_arrow_left), MOV(right_arrow_top), MOV(right_arrow_right), MOV(right_arrow_bottom),
        MOV(right_arrow_left), MOV(up_arrow_arrow), MOV(left_arrow_arrow), MOV(down_arrow_arrow), MOV(right_arrow_arrow) {}
#undef CPY
#undef MOV

keyboard_drawing::~keyboard_drawing() {}

keyboard_drawing & keyboard_drawing::operator=(const keyboard_drawing & from) {
	keyboard_drawing temp(from);
	swap(temp);
	return *this;
}

void keyboard_drawing::swap(keyboard_drawing & with) {
	const auto temp(own_scale);
	own_scale      = with.own_scale;
	with.own_scale = temp;

	border_top.swap(with.border_top);
	border_right.swap(with.border_right);
	border_bottom.swap(with.border_bottom);
	border_left.swap(with.border_left);
	up_arrow_top.swap(with.up_arrow_top);
	up_arrow_right.swap(with.up_arrow_right);
	up_arrow_bottom.swap(with.up_arrow_bottom);
	up_arrow_left.swap(with.up_arrow_left);
	left_arrow_top.swap(with.left_arrow_top);
	left_arrow_right.swap(with.left_arrow_right);
	left_arrow_bottom.swap(with.left_arrow_bottom);
	left_arrow_left.swap(with.left_arrow_left);
	down_arrow_top.swap(with.down_arrow_top);
	down_arrow_right.swap(with.down_arrow_right);
	down_arrow_bottom.swap(with.down_arrow_bottom);
	down_arrow_left.swap(with.down_arrow_left);
	right_arrow_top.swap(with.right_arrow_top);
	right_arrow_right.swap(with.right_arrow_right);
	right_arrow_bottom.swap(with.right_arrow_bottom);
	right_arrow_left.swap(with.right_arrow_left);
	up_arrow_arrow.swap(with.up_arrow_arrow);
	left_arrow_arrow.swap(with.left_arrow_arrow);
	down_arrow_arrow.swap(with.down_arrow_arrow);
	right_arrow_arrow.swap(with.right_arrow_arrow);
}

void keyboard_drawing::draw(RenderTarget & target, RenderStates) const {
	const Vertex * lines_start = static_cast<const Vertex *>(static_cast<const void *>(&border_top));
	const Vertex * triangle_start = static_cast<const Vertex *>(static_cast<const void *>(&up_arrow_arrow));
	target.draw(lines_start, 20 * 2, PrimitiveType::Lines);
	target.draw(triangle_start, 4 * 3, PrimitiveType::Triangles);
}

void keyboard_drawing::move(float x, float y) {
	Vertex * start = static_cast<Vertex *>(static_cast<void *>(&border_top));
	Vector2f offset(x, y);

	for(unsigned int i = 0; i < 2 * 20 + 4 * 3; ++i)
		start[i].position = start[i].position + offset;
}

void keyboard_drawing::scale_size(Vector2f factor) {
	Vertex * start = static_cast<Vertex *>(static_cast<void *>(&border_top));
	factor.x /= 1600.f;
	factor.y /= 900.f;
	own_scale = own_scale * factor;

	for(unsigned int i = 0; i < 2 * 20 + 4 * 3; ++i)
		start[i].position = start[i].position * factor;
}

Vector2f keyboard_drawing::size() const {
	return Vector2f(190, 130) * own_scale;
}
