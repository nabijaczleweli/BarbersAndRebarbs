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


stat_bar::stat_bar(Color color, float filledness) : bar_frame_sprite(load_bar_frame_texture()), bar_fill_sprite(load_bar_fill_texture()), filled(filledness) {
	bar_fill_sprite.setColor(color);
}
stat_bar::stat_bar(const stat_bar & other)
      : Drawable(other), Transformable(other), bar_frame_sprite(other.bar_frame_sprite), bar_fill_sprite(other.bar_fill_sprite),
        filled(min(1.f, abs(other.filled))) {}
stat_bar::stat_bar(stat_bar && other)
      : Drawable(std::move(other)), Transformable(std::move(other)), bar_frame_sprite(std::move(other.bar_frame_sprite)),
        bar_fill_sprite(std::move(other.bar_fill_sprite)), filled(min(1.f, abs(other.filled))) {}

stat_bar & stat_bar::operator=(const stat_bar & from) {
	bar_frame_sprite = from.bar_frame_sprite;
	bar_fill_sprite  = from.bar_fill_sprite;
	filled = min(1.f, abs(from.filled));

	return *this;
}

void stat_bar::draw(RenderTarget & target, RenderStates) const {
	auto rect  = bar_fill_sprite.getTextureRect();
	rect.width = bar_fill_sprite.getTexture()->getSize().x * min(1.f, abs(filled));
	bar_fill_sprite.setTextureRect(static_cast<IntRect>(rect));
	target.draw(bar_fill_sprite);

	target.draw(bar_frame_sprite);
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
