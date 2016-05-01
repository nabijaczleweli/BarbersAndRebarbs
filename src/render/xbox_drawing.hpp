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
#include <SFML/Graphics.hpp>


class xbox_drawing : public sf::Drawable {
private:
	sf::Vector2f own_scale;
	// Clockwise order
	bezier_curve top_left_handle, top_between_handles, top_right_handle, right_right_handle, bottom_right_handle, bottom_between_handles, bottom_left_handle,
	    left_left_handle;

public:
	xbox_drawing(const sf::Vector2f & start);
	xbox_drawing(const sf::Vector2f & start, const sf::Vector2f & size);
	template <class T>
	inline xbox_drawing(const sf::Vector2f & start, const sf::Vector2<T> & size) : xbox_drawing(start, static_cast<sf::Vector2f>(size)) {}
	xbox_drawing(const xbox_drawing & other);
	xbox_drawing(xbox_drawing && other);

	~xbox_drawing();

	xbox_drawing & operator=(const xbox_drawing & from);
	void swap(xbox_drawing & with);

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

	void move(float x, float y);
	void scale_size(sf::Vector2f factor);
	sf::Vector2f size() const;
};
