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
#include "../util/file.hpp"
#include "container.hpp"
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>
#include <utility>


using namespace std;


namespace config_subcategories {
	struct system {
		string & language;
		float & controller_deadzone;

		template <class Archive>
		void serialize(Archive & archive) {
			auto langs = list_files(localization_root);
			transform(begin(langs), end(langs), begin(langs), [](const auto & lang) { return lang.substr(0, lang.find(".lang")); });

			archive(cereal::make_nvp("controller_deadzone", controller_deadzone), cereal::make_nvp("language", language),
			        cereal::make_nvp("available_languages", langs));
		}
	};

	struct application {
		bool & vsync;
		unsigned int & FPS;
		bool & play_sounds;
		unsigned int & splash_length;

		template <class Archive>
		void serialize(Archive & archive) {
			archive(cereal::make_nvp("FPS", FPS), cereal::make_nvp("vsync", vsync), cereal::make_nvp("play_sounds", play_sounds),
			        cereal::make_nvp("splash_length", splash_length));
		}
	};

	struct player {
		float & player_speed;
		string & player_default_firearm;

		template <class Archive>
		void serialize(Archive & archive) {
			archive(cereal::make_nvp("speed", player_speed), cereal::make_nvp("default_firearm", player_default_firearm));
		}
	};
}

template <class Archive>
void serialize(Archive & archive, config & cc) {
	archive(cereal::make_nvp("system", config_subcategories::system{cc.language, cc.controller_deadzone}),
	        cereal::make_nvp("application", config_subcategories::application{cc.vsync, cc.FPS, cc.play_sounds, cc.splash_length}),
	        cereal::make_nvp("player", config_subcategories::player{cc.player_speed, cc.player_default_firearm}));
}


config::config(string && ppath) : path(move(ppath)) {
	ifstream configfile(path);
	if(configfile.is_open()) {
		cereal::JSONInputArchive archive(configfile);
		try {
			archive(*this);
		} catch(...) {
		}
	}
}

config::~config() {
	ofstream configfile(path);
	cereal::JSONOutputArchive archive(configfile);
	archive(cereal::make_nvp(app_name + " configuration", *this));
}
