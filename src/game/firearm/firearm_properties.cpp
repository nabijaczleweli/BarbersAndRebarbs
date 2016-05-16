// The MIT License (MIT)

// Copyright (c) 2016 nabijaczleweli

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


#include "firearm.hpp"
#include "../../reference/container.hpp"
#include "../../util/file.hpp"
#include "jsonpp/parser.hpp"
#include <fstream>
#include <stdexcept>


using namespace std;


static map<string, firearm_properties> all_firearm_properties;


static void load_all(map<string, firearm_properties> & all_props);
static pair<string, firearm_properties> load_single(string && filename);
static firearm_properties::fire_mode_t fire_mode_from_string(const string & name);


const map<string, firearm_properties> & firearm::properties() {
	load_all(all_firearm_properties);
	return all_firearm_properties;
}


static void load_all(map<string, firearm_properties> & all_props) {
	for(auto && fname : list_files(firearm_root))
		all_props.insert(load_single(firearm_root + '/' + fname));
}

static pair<string, firearm_properties> load_single(string && filename) {
	ifstream gun_file(filename);
	json::value doc;
	json::parse(gun_file, doc);

	auto bullet = doc["bullet"].as<json::object>();
	return {doc["id"].as<string>(),
	        {
	            doc["name"].as<string>(),
	            {
	                bullet["speed"].as<float>(),  //
	                bullet["speed_loss"].as<float>(),
	            },
	            fire_mode_from_string(doc["fire_mode"].as<string>()),
	            doc["action_speed"].as<float>(),
	            doc["reload_speed"].as<float>(),
	            doc["mag_size"].as<unsigned int>(),
	        }};
}

firearm_properties::fire_mode_t fire_mode_from_string(const string & name) {
	if(name == "full-auto")
		return firearm_properties::fire_mode_t::full_auto;
	else if(name == "semi-auto")
		return firearm_properties::fire_mode_t::semi_auto;
	else if(name == "single-shot")
		return firearm_properties::fire_mode_t::single_shot;
	else
		throw domain_error("\"" + name + "\" is not a fire_mode");
}


static const map<string, firearm_properties> & ____ = firearm::properties();
