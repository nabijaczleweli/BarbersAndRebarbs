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


#ifndef ENTITY_HPP
#define ENTITY_HPP


#include "cpp-nbt/tags/complex/nbt_compound.hpp"
#include "cpp-nbt/nbt_reloadable.hpp"


class entity : public cpp_nbt::nbt_reloadable {
protected:
	float x, y;
	float motion_x, motion_y;

public:
	entity();
	entity(const cpp_nbt::nbt_compound & from);
	entity(const entity & other);
	entity(entity && other);

	virtual ~entity();

	virtual void read_from_nbt(const cpp_nbt::nbt_compound & from);
	virtual void write_to_nbt(cpp_nbt::nbt_compound & to) const;

	virtual void tick(float max_x = 0, float max_y = 0);  // maxes for physics


	void start_movement(float amt_x, float amt_y);

	virtual float speed() const;
};


#endif  // ENTITY_HPP
