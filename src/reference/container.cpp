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


#include "container.hpp"
#include <whereami++.hpp>


const std::string assets_root(whereami::executable_dir() + "/assets");
const std::string textures_root(assets_root + "/textures");
const std::string font_root(assets_root + "/fonts");
const std::string sound_root(assets_root + "/sound");
const std::string localization_root(assets_root + "/lang");
const std::string drawing_root(assets_root + "/drawings");
const std::string firearm_root(assets_root + "/guns");

const std::string app_name("BarbersAndRebarbs");
/***/ config app_configuration(whereami::executable_dir() + "/" + app_name + ".cfg");

const cpp_localiser::localiser fallback_iser(localization_root);
const cpp_localiser::localiser local_iser(localization_root, app_configuration.language);
const cpp_localiser::localiser global_iser(local_iser, fallback_iser);


const sf::Font font_pixelish([] {
	sf::Font tmp;
	tmp.loadFromFile(font_root + "/04B_30.ttf");
	return tmp;
}());
const sf::Font font_swirly([] {
	sf::Font tmp;
	tmp.loadFromFile(font_root + "/MACABRA_.ttf");
	return tmp;
}());
const sf::Font font_monospace([] {
	sf::Font tmp;
	tmp.loadFromFile(font_root + "/DejaVuSansMono.ttf");
	return tmp;
}());


const audiere::AudioDevicePtr audio_device(audiere::OpenDevice());
