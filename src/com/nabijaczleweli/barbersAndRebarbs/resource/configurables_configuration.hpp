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
#ifndef CONFIGURABLES_CONFIG_HPP
#define CONFIGURABLES_CONFIG_HPP


#include "../util/configurable.hpp"
#include <cpponfiguration/configuration.hpp>
#include <unordered_set>


class configurables_configuration : public cpponfig::configuration,
                                    cpponfig::swappable<configurables_configuration>,
                                    public cpponfig::hashable<configurables_configuration> {
private:
	std::unordered_set<configurable *> configurables;

	virtual size_t hash_code() const override;

public:
	configurables_configuration();
	configurables_configuration(const std::string & name);
	configurables_configuration(const configurables_configuration & other);
	configurables_configuration(configurables_configuration && other);

	virtual ~configurables_configuration();

	configurables_configuration & operator=(const configurables_configuration & other);
	configurables_configuration & operator+=(const configurables_configuration & other);
	configurables_configuration & operator-=(const configurables_configuration & other);

	virtual void swap(configurables_configuration & cfg) override;

	void add(configurable * cfgbl);
	inline void add(configurable & cfgbl) { add(&cfgbl); }
	void configure();
};

configurables_configuration operator+(const configurables_configuration & lhs, const configurables_configuration & rhs);
configurables_configuration operator-(const configurables_configuration & lhs, const configurables_configuration & rhs);


#endif  // CONFIGURABLES_CONFIG_HPP
