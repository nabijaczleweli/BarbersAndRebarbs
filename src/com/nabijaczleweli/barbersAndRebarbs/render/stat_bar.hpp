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


#ifndef STAT_BAR_HPP
#define STAT_BAR_HPP


#include <SFML/Graphics.hpp>


class stat_bar : public sf::Drawable, public sf::Transformable {
protected:
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
	static sf::Texture bar_frame_texture;
	static const sf::Texture & load_bar_frame_texture();
	static sf::Texture bar_fill_texture;
	static const sf::Texture & load_bar_fill_texture();

	sf::Sprite bar_frame_sprite;
	sf::Sprite bar_fill_sprite;

public:
	float filled;

	stat_bar(sf::Color color, float filledness);
	stat_bar(const stat_bar & other);
	stat_bar(stat_bar && other);

	stat_bar & operator=(const stat_bar & from);

	inline sf::FloatRect getGlobalBounds() const { return bar_frame_sprite.getGlobalBounds(); }
	inline sf::FloatRect getLocalBounds() const { return bar_frame_sprite.getLocalBounds(); }

	virtual void setPosition(float x, float y) /*override*/;
	virtual void setPosition(const sf::Vector2f & position) /*override*/;
};


#endif  // STAT_BAR_HPP
