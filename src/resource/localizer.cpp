// The MIT License (MIT)

// Copyright (c) 2014 nabijaczleweli

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


#include "localizer.hpp"
#include "../reference/container.hpp"
#include "../util/strings.hpp"
#include <fstream>


using namespace std;


localizer::localizer() : localizer("en_US") {}

localizer::localizer(nothrow_t) {}

localizer::localizer(istream & in) {
	init(in);
}

localizer::localizer(const string & locale) {
	ifstream in(localization_root + '/' + locale + ".lang");
	if(in.is_open())
		init(in);
	end = language.cend();
}

localizer::localizer(localizer && loc) : language(loc.language) {}
localizer::localizer(const localizer & loc) : language(loc.language) {}
localizer::localizer(const localizer & loc0, const localizer & loc1) : localizer(loc0) {
	merge(loc1);
}

/** Adds keys from `loc` for which there are no elements in `this` */
localizer & localizer::merge(const localizer & loc) {
	language.insert(loc.language.begin(), loc.language.cend());
	end = language.cend();
	return *this;
}

localizer & localizer::open(const string & loc) {
	return merge(localizer(loc));
}

void localizer::init(istream & in) {
	string line;
	while(getline(in, line)) {
		if(!line.size())
			continue;
		ltrim(line);
		if(line[0] == '#')
			continue;
		const auto idx = line.find('=');
		if(idx == string::npos)
			continue;
		language.emplace(rtrim(line.substr(0, idx)), trim(line.substr(idx + 1)));
	}
	end = language.cend();
}

bool localizer::has_any_key() const {
	return !language.empty();
}

bool localizer::can_translate_key(const string & key) const {
	return !language.empty() && language.count(key) > 0;
}

const string & localizer::translate_key(const string & key) const {
	const auto & itr = language.find(key);
	if(itr == end)
		return key;
	else
		return itr->second;
}
