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


#include "xbox_drawing.hpp"


using namespace std;
using namespace sf;


template <class T>
inline Vector2<T> operator*(const Vector2<T> & lhs, const Vector2<T> & rhs) {
	Vector2<T> temp(lhs);
	temp.x *= rhs.x;
	temp.y *= rhs.y;
	return temp;
}


// WIDTH = 315.333514
// HEIGHT = 204.00709
xbox_drawing::xbox_drawing(const Vector2f & s)
      : own_scale(1, 1), top_left_handle(Vector2f(33, 11) + s, Vector2f(62, -10) + s, Vector2f(93, 13) + s),
        top_between_handles(Vector2f(93, 13) + s, Vector2f(153, 30) + s, Vector2f(223, 13) + s),
        top_right_handle(Vector2f(223, 13) + s, Vector2f(254, -10) + s, Vector2f(283, 11) + s),
        right_right_handle(Vector2f(283, 11) + s, Vector2f(325, 42) + s, Vector2f(313, 150) + s),
        bottom_right_handle(Vector2f(313, 150) + s, Vector2f(296, 239) + s, Vector2f(236, 184) + s),
        bottom_between_handles(Vector2f(236, 184) + s, Vector2f(151, 154) + s, Vector2f(80, 184) + s),
        bottom_left_handle(Vector2f(80, 184) + s, Vector2f(20, 239) + s, Vector2f(3, 150) + s),
        left_left_handle(Vector2f(3, 150) + s, Vector2f(-9, 42) + s, Vector2f(33, 11) + s) {}
xbox_drawing::xbox_drawing(const Vector2f & start, const Vector2f & size) : xbox_drawing(start) {
	scale_size(size);
}
#define CPY(field) field(other.field)
#define MOV(field) field(std::move(other.field))
xbox_drawing::xbox_drawing(const xbox_drawing & other)
      : CPY(own_scale), CPY(top_left_handle), CPY(top_between_handles), CPY(top_right_handle), CPY(right_right_handle), CPY(bottom_right_handle),
        CPY(bottom_between_handles), CPY(bottom_left_handle), CPY(left_left_handle) {}
xbox_drawing::xbox_drawing(xbox_drawing && other)
      : MOV(own_scale), MOV(top_left_handle), MOV(top_between_handles), MOV(top_right_handle), MOV(right_right_handle), MOV(bottom_right_handle),
        MOV(bottom_between_handles), MOV(bottom_left_handle), MOV(left_left_handle) {}
#undef CPY
#undef MOV

xbox_drawing::~xbox_drawing() {}

xbox_drawing & xbox_drawing::operator=(const xbox_drawing & from) {
	xbox_drawing temp(from);
	swap(temp);
	return *this;
}

void xbox_drawing::swap(xbox_drawing & with) {
	const auto temp(own_scale);
	own_scale      = with.own_scale;
	with.own_scale = temp;

	top_left_handle.swap(with.top_left_handle);
	top_between_handles.swap(with.top_between_handles);
	top_right_handle.swap(with.top_right_handle);
	right_right_handle.swap(with.right_right_handle);
	bottom_right_handle.swap(with.bottom_right_handle);
	bottom_between_handles.swap(with.bottom_between_handles);
	bottom_left_handle.swap(with.bottom_left_handle);
	left_left_handle.swap(with.left_left_handle);
}

void xbox_drawing::draw(RenderTarget & target, RenderStates) const {
	target.draw(top_left_handle);
	target.draw(top_between_handles);
	target.draw(top_right_handle);
	target.draw(right_right_handle);
	target.draw(bottom_right_handle);
	target.draw(bottom_between_handles);
	target.draw(bottom_left_handle);
	target.draw(left_left_handle);
}

void xbox_drawing::move(float x, float y) {
	top_left_handle.move(x, y);
	top_between_handles.move(x, y);
	top_right_handle.move(x, y);
	right_right_handle.move(x, y);
	bottom_right_handle.move(x, y);
	bottom_between_handles.move(x, y);
	bottom_left_handle.move(x, y);
	left_left_handle.move(x, y);
}

void xbox_drawing::scale_size(Vector2f factor) {
	factor.x /= 1600.f;
	factor.y /= 900.f;
	own_scale = own_scale * factor;

	top_left_handle.scale(factor);
	top_between_handles.scale(factor);
	right_right_handle.scale(factor);
	bottom_right_handle.scale(factor);
	bottom_between_handles.scale(factor);
	bottom_left_handle.scale(factor);
	left_left_handle.scale(factor);
}

Vector2f xbox_drawing::size() const {
	return Vector2f(315.333514, 204.00709) * own_scale;
}
