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


#include "joystick_info.hpp"


using namespace sf;


const joystick_button_mappings X360_button_mappings({
	{"A", 0},
	{"B", 1},
	{"X", 2},
	{"Y", 3},
	{"LB", 4},
	{"RB", 5},
	{"Back", 6},
	{"Start", 7},
	{"LeftStick", 8},
	{"RightStick", 9}
});

const joystick_axis_mappings X360_axis_mappings({
	{"LeftStickHorizontal", {Joystick::X, 1, -1}},
	{"LeftStickVertical", {Joystick::Y, -1, 1}},
	{"Triggers", {Joystick::Z, -1, 1}},
	{"RightStickVertical", {Joystick::R, -1, 1}},
	{"RightStickHorizontal", {Joystick::U, 1, -1}},
	{"DPadHorizontal", {Joystick::PovX, 1, -1}},
	{"DPadVertical", {Joystick::PovY, 1, -1}}
});
