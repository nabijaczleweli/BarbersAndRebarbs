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


#include "stat_bar.hpp"
#include "../reference/container.hpp"
#include <utility>
#include <cmath>


using namespace std;
using namespace sf;


const Texture & stat_bar::load_bar_frame_texture() {
	static Texture bar_frame_texture;
	static bool bar_frame_texture_loaded = false;

	if(!bar_frame_texture_loaded)
		bar_frame_texture_loaded = bar_frame_texture.loadFromFile(textures_root + "/gui/game/stat_bar_frame.png");
	return bar_frame_texture;
}

const Texture & stat_bar::load_bar_fill_texture() {
	static Texture bar_fill_texture;
	static bool bar_fill_texture_loaded = false;

	if(!bar_fill_texture_loaded)
		bar_fill_texture_loaded = bar_fill_texture.loadFromFile(textures_root + "/gui/game/stat_bar_fill.png");
	return bar_fill_texture;
}


stat_bar::stat_bar() : filledness(nullptr) {}
stat_bar::stat_bar(Color color, const float & filledness_r)
      : bar_frame_sprite(load_bar_frame_texture()), bar_fill_sprite(load_bar_fill_texture()), filledness(&filledness_r) {
	bar_fill_sprite.setColor(color);
}

void stat_bar::draw(RenderTarget & target, RenderStates) const {
	if(filledness) {
		auto rect  = bar_fill_sprite.getTextureRect();
		rect.width = bar_fill_sprite.getTexture()->getSize().x * min(1.f, abs(*filledness));
		bar_fill_sprite.setTextureRect(static_cast<IntRect>(rect));
		target.draw(bar_fill_sprite);
	}

	target.draw(bar_frame_sprite);
}

FloatRect stat_bar::getGlobalBounds() const {
	return bar_frame_sprite.getGlobalBounds();
}
FloatRect stat_bar::getLocalBounds() const {
	return bar_frame_sprite.getLocalBounds();
}

void stat_bar::setPosition(float x, float y) {
	Transformable::setPosition(x, y);
	bar_frame_sprite.setPosition(x, y);
	bar_fill_sprite.setPosition(x, y);
}

void stat_bar::setPosition(const Vector2f & position) {
	Transformable::setPosition(position);
	bar_frame_sprite.setPosition(position);
	bar_fill_sprite.setPosition(position);
}
