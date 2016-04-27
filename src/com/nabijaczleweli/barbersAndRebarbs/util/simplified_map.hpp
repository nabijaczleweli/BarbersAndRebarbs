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


#ifndef JOYSTICK_MAPPINGS_HPP
#define JOYSTICK_MAPPINGS_HPP


#include <unordered_map>


template <class K, class V>
class simplified_map {
private:
	typedef std::unordered_map<K, V> mapping_map;
	typedef typename mapping_map::value_type mapping_single;

	const mapping_map * mappings;

public:
	simplified_map(std::initializer_list<mapping_single> init) : mappings(new mapping_map(init)) {}
	simplified_map(const simplified_map<K, V> & other) : mappings(new mapping_map(*other.mappings)) {}
	simplified_map(simplified_map<K, V> && other) : mappings(std::move(other.mappings)) { other.mappings = nullptr; }

	~simplified_map() {
		delete mappings;
		mappings = nullptr;
	}

	simplified_map<K, V> & operator=(const simplified_map<K, V> & from) {
		simplified_map<K, V> temp(from);
		swap(temp);
		return *this;
	}

	void swap(simplified_map<K, V> & with) {
		mapping_map * temp = mappings;
		mappings           = with.mappings;
		with.mappings      = temp;
	}


	inline V operator[](const K & key) const { return mappings->at(key); }

	inline V operator[](K && key) const { return mappings->at(std::move(key)); }

	inline const mapping_map & get() const { return *mappings; }
};


#endif  // JOYSTICK_MAPPINGS_HPP
