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


#include "main_game_screen.hpp"
#include "../../../reference/container.hpp"
#include "../../../resource/localizer.hpp"
#include "../../application.hpp"


using namespace std;
using namespace sf;


void main_game_screen::config(cpponfig::configuration &) {}

void main_game_screen::setup() {
	screen::setup();
	const auto & winsize(app->window.getView().getSize());
	const auto & hp_bounds(hp_stat.getLocalBounds());
	const auto & energy_bounds(energy_stat.getLocalBounds());

	hp_stat.setPosition(winsize.x / 4 - hp_bounds.width / 2, (59.f / 60.f) * winsize.y - hp_bounds.height / 2);
	energy_stat.setPosition((winsize.x / 4) * 3 - energy_bounds.width / 2, (59.f / 60.f) * winsize.y - energy_bounds.height / 2);
}

int main_game_screen::loop() {
	for(auto & entity : entities)
		entity->tick(app->window.getSize().x, app->window.getSize().y);
	the_player.tick(app->window.getSize().x, app->window.getSize().y);
	return 0;
}

int main_game_screen::draw() {
	const Drawable * drwbl = nullptr;
	for(const auto & entity : entities)
		if((drwbl = dynamic_cast<const Drawable *>(entity.get())))
			app->window.draw(*drwbl);
	app->window.draw(the_player);
	app->window.draw(hp_stat);
	app->window.draw(energy_stat);
	return 0;
}

int main_game_screen::handle_event(const Event & event) {
	if(int i = screen::handle_event(event))
		return i;

	return 0;
}

main_game_screen::main_game_screen(application * theapp) : screen(theapp), hp_stat(Color::Red, 1), energy_stat(Color(50, 200, 200), 1) {}
main_game_screen::main_game_screen(const main_game_screen & other)
      : screen(other), configurable(other), hp_stat(other.hp_stat), energy_stat(other.energy_stat) {}
main_game_screen::main_game_screen(main_game_screen && other)
      : screen(move(other)), configurable(move(other)), hp_stat(move(other.hp_stat)), energy_stat(move(other.energy_stat)) {}

main_game_screen::~main_game_screen() {}
