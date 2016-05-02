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
#include "whereami++.hpp"


using namespace sf;
using namespace std;
using namespace audiere;
using namespace whereami;


const string assets_root(executable_dir() + "/assets");
const string textures_root(assets_root + "/textures");
const string font_root(assets_root + "/fonts");
const string sound_root(assets_root + "/sound");
const string localization_root(assets_root + "/lang");

const string app_name("BarbersAndRebarbs");
/***/ config app_configuration(executable_dir() + "/" + app_name + ".cfg");

const localizer fallback_izer;
const localizer local_izer(app_configuration.language);
const localizer global_izer(local_izer, fallback_izer);


const Font font_pixelish([] {
	Font tmp;
	tmp.loadFromFile(font_root + "/04B_30.ttf");
	return tmp;
}());
const Font font_swirly([] {
	Font tmp;
	tmp.loadFromFile(font_root + "/MACABRA_.ttf");
	return tmp;
}());


const AudioDevicePtr audio_device(OpenDevice());
