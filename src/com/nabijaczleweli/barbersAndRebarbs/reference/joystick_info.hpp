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


#ifndef JOYSTICK_INFO_HPP
#define JOYSTICK_INFO_HPP


#include "../util/simplified_map.hpp"
#include <SFML/Window.hpp>


struct directional_axis {
	sf::Joystick::Axis axis;
	int up_right, down_left;
};

template<class T>
using joystick_mappings = simplified_map<std::string, T>;

typedef joystick_mappings<unsigned int> joystick_button_mappings;
typedef joystick_mappings<directional_axis> joystick_axis_mappings;


extern const joystick_button_mappings X360_button_mappings;
extern const joystick_axis_mappings X360_axis_mappings;


#endif  // JOYSTICK_INFO_HPP
