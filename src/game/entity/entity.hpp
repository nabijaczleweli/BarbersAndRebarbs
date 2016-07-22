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


#include <utility>
#include <jsonpp/value.hpp>


class game_world;
class entity {
protected:
	float x, y;
	float motion_x, motion_y;

	std::size_t id;
	game_world & world;

public:
	entity(game_world & world, std::size_t id);
	entity(game_world & world, std::size_t id, const json::object & from);

	virtual ~entity() = default;

	virtual void read_from_json(const json::object & from);
	virtual json::object write_to_json() const;

	virtual void tick(float max_x = 0, float max_y = 0);  // maxes for physics


	void start_movement(float amt_x, float amt_y);

	virtual float speed() const;
	virtual float speed_loss() const;
};
