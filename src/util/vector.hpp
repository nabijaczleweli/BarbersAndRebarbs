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


#include <SFML/System.hpp>
#include <type_traits>


template <class T>
sf::Vector2<T> normalised(const sf::Vector2<T> & vec) {
	const T length = sqrt(vec.x * vec.x + vec.y * vec.y);
	if(length)
		return vec / length;
	return vec;
}

template <class T, class U, class R = std::common_type_t<T, U>>
inline sf::Vector2<R> operator*(const sf::Vector2<T> & lhs, const sf::Vector2<U> & rhs) {
	sf::Vector2<R> temp(lhs);
	temp.x *= rhs.x;
	temp.y *= rhs.y;
	return temp;
}

template <class T, class U, class R = std::common_type_t<T, U>>
inline sf::Vector2<R> operator/(const sf::Vector2<T> & lhs, const sf::Vector2<U> & rhs) {
	sf::Vector2<R> temp(lhs);
	temp.x /= rhs.x;
	temp.y /= rhs.y;
	return temp;
}
