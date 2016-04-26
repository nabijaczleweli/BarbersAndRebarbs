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


#ifndef KEYBOARD_DRAWING_HPP
#define KEYBOARD_DRAWING_HPP


#include <SFML/Graphics.hpp>


class keyboard_drawing : public sf::Drawable {
	public:
		typedef std::pair<sf::Vertex, sf::Vertex> line;
		typedef std::tuple<sf::Vertex, sf::Vertex, sf::Vertex> triangle;

	private:
		sf::Vector2f own_scale;
		// Clockwise order
		line border_top,
		     border_right,
		     border_bottom,
		     border_left,
		     up_arrow_top,
		     up_arrow_right,
		     up_arrow_bottom,
		     up_arrow_left,
		     left_arrow_top,
		     left_arrow_right,
		     left_arrow_bottom,
		     left_arrow_left,
		     down_arrow_top,
		     down_arrow_right,
		     down_arrow_bottom,
		     down_arrow_left,
		     right_arrow_top,
		     right_arrow_right,
		     right_arrow_bottom,
		     right_arrow_left;
		triangle up_arrow_arrow,
		         left_arrow_arrow,
		         down_arrow_arrow,
		         right_arrow_arrow;

	public:
		keyboard_drawing(const sf::Vector2f & start);
		keyboard_drawing(const sf::Vector2f & start, const sf::Vector2f & size);
		template<class T>
		inline keyboard_drawing(const sf::Vector2f & start, const sf::Vector2<T> & size) : keyboard_drawing(start, static_cast<sf::Vector2f>(size)) {}
		keyboard_drawing(const keyboard_drawing & other);
		keyboard_drawing(keyboard_drawing && other);

		~keyboard_drawing();

		keyboard_drawing & operator=(const keyboard_drawing & from);
		void swap(keyboard_drawing & with);

		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

		void move(float x, float y);
		void scale_size(sf::Vector2f factor);
		sf::Vector2f size() const;
};


#endif  // KEYBOARD_DRAWING_HPP
