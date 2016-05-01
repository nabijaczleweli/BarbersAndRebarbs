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


#include "resource_location.hpp"
#include "../reference/container.hpp"
#include "../util/file.hpp"
#include <algorithm>
#include <stdexcept>


using namespace std;


resource_location::resource_location(const string & domain, const string & path) : resource_domain(domain), resource_path(path) {
	if(resource_domain.empty())
		throw invalid_argument("Invalid resource_domain: " + resource_domain);
}

resource_location::resource_location(const string & bundle) {
	string domain(app_name);
	string path(bundle);
	auto colon_id = bundle.find(':');

	if(colon_id != string::npos) {
		path = bundle.substr(colon_id + 1, bundle.length());

		if(colon_id)
			domain = bundle.substr(0, colon_id);
	}

	transform(domain.begin(), domain.end(), domain.begin(), ::tolower);
	const_cast<string &>(resource_domain) = domain;
	const_cast<string &>(resource_path) = path;
}

resource_location::operator string() const {
	return resource_domain + ":" + resource_path;
}

bool resource_location::operator==(const resource_location & other) const {
	if(&other == this)
		return true;
	else
		return resource_domain == other.resource_domain && resource_path == other.resource_path;
}

string resource_location::path() const {
	return assets_root + '/' + resource_domain + '/' + resource_path;
}

bool resource_location::exists() const {
	return file_exists(path());
}
