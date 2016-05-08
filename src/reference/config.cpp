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


#include "config.hpp"
#include <fstream>
#include <utility>
#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/archives/json.hpp"
#include "container.hpp"
#include "../util/file.hpp"


using namespace std;


template <class Archive>
void serialize(Archive & archive, config & cc) {
	auto langs = list_files(localization_root);
	langs.erase(remove_if(begin(langs), end(langs), [](const auto & lang) { return lang[0] == '.'; }), end(langs));
	transform(begin(langs), end(langs), begin(langs), [](const auto & lang) { return lang.substr(0, lang.find(".lang")); });

	archive(cereal::make_nvp("application:FPS", cc.FPS), cereal::make_nvp("application:vsync", cc.vsync),
	        cereal::make_nvp("application:play_sounds", cc.play_sounds), cereal::make_nvp("application:splash_length", cc.splash_length),
	        cereal::make_nvp("system:controller_deadzone", cc.controller_deadzone), cereal::make_nvp("system:language", cc.language),
	        cereal::make_nvp("system:available_languages", langs), cereal::make_nvp("player:speed", cc.player_speed));
}

config::config(string && ppath) : path(move(ppath)) {
	ifstream configfile(path);
	if(configfile.is_open()) {
		cereal::JSONInputArchive archive(configfile);
		try {
			archive(*this);
		} catch(cereal::RapidJSONException &) {
		} catch(cereal::Exception &) {
		}
	}
}

config::~config() {
	ofstream configfile(path);
	cereal::JSONOutputArchive archive(configfile);
	archive(cereal::make_nvp(app_name + " configuration", *this));
}
