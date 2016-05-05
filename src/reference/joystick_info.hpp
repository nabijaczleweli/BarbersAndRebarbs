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


#pragma once


#include <SFML/Window.hpp>


namespace X360_button_mappings {
	static const constexpr auto A          = 0u;
	static const constexpr auto B          = 1u;
	static const constexpr auto X          = 2u;
	static const constexpr auto Y          = 3u;
	static const constexpr auto LB         = 4u;
	static const constexpr auto RB         = 5u;
	static const constexpr auto Back       = 6u;
	static const constexpr auto Start      = 7u;
	static const constexpr auto LeftStick  = 8u;
	static const constexpr auto RightStick = 9u;
}

namespace X360_axis_mappings {
	static const constexpr auto LeftStickHorizontal  = sf::Joystick::X;
	static const constexpr auto LeftStickVertical    = sf::Joystick::Y;
	static const constexpr auto Triggers             = sf::Joystick::Z;
	static const constexpr auto RightStickVertical   = sf::Joystick::R;
	static const constexpr auto RightStickHorizontal = sf::Joystick::U;
	static const constexpr auto DPadHorizontal       = sf::Joystick::PovX;
	static const constexpr auto DPadVertical         = sf::Joystick::PovY;
}

namespace X360_axis_up_right_mappings {
	static const constexpr auto LeftStickHorizontal  = 1;
	static const constexpr auto LeftStickVertical    = -1;
	static const constexpr auto Triggers             = -1;
	static const constexpr auto RightStickVertical   = -1;
	static const constexpr auto RightStickHorizontal = 1;
	static const constexpr auto DPadHorizontal       = 1;
	static const constexpr auto DPadVertical         = 1;
}

namespace X360_axis_down_left_mappings {
	static const constexpr auto LeftStickHorizontal  = -1;
	static const constexpr auto LeftStickVertical    = 1;
	static const constexpr auto Triggers             = 1;
	static const constexpr auto RightStickVertical   = 1;
	static const constexpr auto RightStickHorizontal = -1;
	static const constexpr auto DPadHorizontal       = -1;
	static const constexpr auto DPadVertical         = -1;
}
