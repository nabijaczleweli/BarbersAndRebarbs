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


#include "configurables_configuration.hpp"
#include <cpponfiguration/configuration.hpp>


using namespace std;
using namespace cpponfig;


configurables_configuration::configurables_configuration() : configuration() {}
configurables_configuration::configurables_configuration(const std::string & name) : configuration(name) {}
configurables_configuration::configurables_configuration(const configurables_configuration & other) : configuration(other) {}
configurables_configuration::configurables_configuration(configurables_configuration && other) : configuration(move(other)) {}

configurables_configuration::~configurables_configuration() {
	save();
}

configurables_configuration & configurables_configuration::operator=(const configurables_configuration & other) {
	configurables_configuration temp(other);
	swap(temp);
	return *this;
}

configurables_configuration & configurables_configuration::operator+=(const configurables_configuration & other) {
	configuration::operator+=(other);
	configurables.insert(other.configurables.begin(), other.configurables.end());

	return *this;
}

configurables_configuration & configurables_configuration::operator-=(const configurables_configuration & other) {
	configuration::operator-=(other);
	for(const auto & cfgbl : other.configurables)
		configurables.erase(cfgbl);

	return *this;
}

size_t configurables_configuration::hash_code() const {
	#define COLHASH(col, hash, prime) if(col.empty()) \
	                             result ^= prime; \
	                           else \
	                             for(const auto & elem : col) \
	                                result ^= hash(elem);

	static const salt slt;
	static const hash<configurable *> cfgblptr_hash;

	size_t result = 0x1E92A67D;

	COLHASH(configurables, cfgblptr_hash, 0x1FC4F0C5)

	return result ^ configuration::hash_code() ^ slt;

	#undef COLHASH
}

void configurables_configuration::swap(configurables_configuration & cfg) {
	configuration::swap(cfg);
	configurables.swap(cfg.configurables);
}

void configurables_configuration::add(configurable * cfgbl) {
	configurables.emplace(cfgbl);
}

void configurables_configuration::configure() {
	if(!configurables.empty()) {
		for(auto cfgbl : configurables)
			cfgbl->config(*this);
		configurables.clear();
	}
}


configurables_configuration operator+(const configurables_configuration & lhs, const configurables_configuration & rhs) {
	configurables_configuration temp(lhs);
	temp += rhs;
	return temp;
}

configurables_configuration operator-(const configurables_configuration & lhs, const configurables_configuration & rhs) {
	configurables_configuration temp(lhs);
	temp -= rhs;
	return temp;
}
