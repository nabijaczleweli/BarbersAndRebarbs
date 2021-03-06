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


#include "main_menu_screen.hpp"
#include "../../../game/firearm/firearm.hpp"
#include "../../../reference/container.hpp"
#include "../../../reference/joystick_info.hpp"
#include "../../../util/file.hpp"
#include "../../../util/sound.hpp"
#include "../../../util/url.hpp"
#include "../../../util/zstd.hpp"
#include "../../application.hpp"
#include "../game/main_game_screen.hpp"
#include <cmath>
#include <fmt/format.h>
#include <iostream>
#include <jsonpp/parser.hpp>
#include <semver/semver200.h>


using namespace std::literals;


void main_menu_screen::move_selection(main_menu_screen::direction dir, bool end = false) {
	const auto max_idx     = main_buttons.size() - 1;
	auto desired_selection = selected;

	switch(dir) {
		case direction::up:
			if(end)
				desired_selection = max_idx;
			else
				desired_selection = selected + 1;
			break;
		case direction::down:
			if(selected && !end)
				desired_selection = selected - 1;
			else
				desired_selection = 0;
			break;
	}

	desired_selection = std::min(max_idx, desired_selection);
	if(app_configuration.play_sounds) {
		if(desired_selection != selected)
			selected_option_switch_sound->play();
		else
			selected_option_unchanged_sound->play();
	}
	selected = desired_selection;
}

void main_menu_screen::press_button() {
	if(app_configuration.play_sounds)
		selected_option_select_sound->play();
	auto itr = main_buttons.begin();
	advance(itr, selected);
	(itr->second)(itr->first);
}

void main_menu_screen::try_drawings() {
	if(!(control_frames_counter++ % 10)) {
		control_frames_counter = 0;
		if(joystick_drawing.first != sf::Joystick::isConnected(0)) {
			joystick_drawing.first ^= 1;
			joystick_drawing.second.move(0, joystick_drawing.second.size().y * .55f * (joystick_drawing.first ? 1 : -1));
			keys_drawing.move(0, joystick_drawing.second.size().y * .55f * (joystick_drawing.first ? -1 : 1));
		}
	}
}

void main_menu_screen::load_game(sf::Text & txt, const std::string & save_path) {
	const auto data = decompress_file_to_string(save_path);

	if(!std::get<1>(data))
		txt.setString(global_iser.translate_key("gui.main_menu.text.load_file_inaccessible"));
	else if(const auto err_s = std::get<2>(data))
		txt.setString(fmt::format(global_iser.translate_key("gui.main_menu.text.load_decompression_error"), err_s));
	else {
		json::value save;
		json::parse(std::get<0>(data), save);
		app.schedule_screen<main_game_screen>(save.as<json::object>());
	}
}


void main_menu_screen::set_default_menu_items() {
	main_buttons.clear();

	main_buttons.emplace_front(sf::Text(global_iser.translate_key("gui.main_menu.text.start"), font_swirly),
	                           [&](sf::Text &) { app.schedule_screen<main_game_screen>(); });
	main_buttons.emplace_front(sf::Text(global_iser.translate_key("gui.main_menu.text.load"), font_swirly), [&](sf::Text & txt) {
		main_buttons.clear();
		main_buttons.emplace_front(sf::Text(global_iser.translate_key("gui.main_menu.text.back"), font_swirly), [&](sf::Text &) { set_default_menu_items(); });
		for(auto && fname : [] {
			    auto saves = list_files(saves_root);
			    std::sort(saves.begin(), saves.end());
			    return saves;
			  }())
			main_buttons.emplace_front(sf::Text(fname.substr(0, fname.rfind('.')), font_swirly),
			                           [&, fname = fname ](sf::Text &) { load_game(txt, saves_root + '/' + fname); });
		selected = main_buttons.size() - 1;
	});
	main_buttons.emplace_front(sf::Text(global_iser.translate_key("gui.main_menu.text."s + (app_configuration.play_sounds ? "" : "un") + "mute"), font_swirly),
	                           [&](sf::Text & txt) {
		                           app_configuration.play_sounds = !app_configuration.play_sounds;
		                           txt.setString(global_iser.translate_key("gui.main_menu.text."s + (app_configuration.play_sounds ? "" : "un") + "mute"));
		                           app.retry_music();
		                         });
	main_buttons.emplace_front(sf::Text(global_iser.translate_key("gui.main_menu.text.configure"), font_swirly), [&](sf::Text &) { set_config_menu_items(); });
	main_buttons.emplace_front(sf::Text(global_iser.translate_key("gui.main_menu.text.quit"), font_swirly), [&](sf::Text &) { app.window.close(); });

	selected = main_buttons.size() - 1;
}

void main_menu_screen::set_config_menu_items() {
	main_buttons.clear();

	bool music_volume_changed      = false;
	const auto langs               = config::available_languages();
	const std::size_t cur_lang_idx = std::distance(langs.begin(), std::find(langs.begin(), langs.end(), app_configuration.language));
	const auto volume_func         = [&](const char * key, float & volume, auto additional) {
		return [&, key, additional](sf::Text & text) {
			volume += .05f;
			if(volume > 1.f)
				volume = 0.f;

			additional(volume);
			text.setString(fmt::format(global_iser.translate_key(key), volume * 100.f));
		};
	};

	main_buttons.emplace_front(sf::Text(global_iser.translate_key("gui.main_menu.text.back"), font_swirly), [&](sf::Text &) {
		local_iser  = cpp_localiser::localiser(localization_root, app_configuration.language);
		global_iser = cpp_localiser::localiser(local_iser, fallback_iser);
		if(music_volume_changed)
			app.retry_music();
		set_default_menu_items();
	});
	main_buttons.emplace_front(sf::Text(fmt::format(global_iser.translate_key("gui.main_menu.text.config_lang"), app_configuration.language), font_pixelish, 20),
	                           [&, langs = std::move(langs), idx = cur_lang_idx ](sf::Text & text) mutable {
		                           ++idx;
		                           if(idx == langs.size())
			                           idx = 0;

		                           app_configuration.language = langs[idx];
		                           text.setString(fmt::format(global_iser.translate_key("gui.main_menu.text.config_lang"), app_configuration.language));
		                         });
	main_buttons.emplace_front(
	    sf::Text(fmt::format(global_iser.translate_key("gui.main_menu.text.config_controller_deadzone"), app_configuration.controller_deadzone), font_pixelish,
	             20),
	    [&](sf::Text & text) {
		    app_configuration.controller_deadzone += 5;
		    if(app_configuration.controller_deadzone > 100)
			    app_configuration.controller_deadzone = 0;

		    text.setString(fmt::format(global_iser.translate_key("gui.main_menu.text.config_controller_deadzone"), app_configuration.controller_deadzone));
		  });
	main_buttons.emplace_front(
	    sf::Text(global_iser.translate_key("gui.main_menu.text.config_vsync_"s + (app_configuration.vsync ? "on" : "off")), font_pixelish, 20),
	    [&](sf::Text & text) {
		    app_configuration.vsync ^= 1;
		    text.setString(global_iser.translate_key("gui.main_menu.text.config_vsync_"s + (app_configuration.vsync ? "on" : "off")));
		  });
	main_buttons.emplace_front(sf::Text(fmt::format(global_iser.translate_key("gui.main_menu.text.config_fps"), app_configuration.FPS), font_pixelish, 20),
	                           [&](sf::Text & text) {
		                           app_configuration.FPS += 30;
		                           if(app_configuration.FPS > 120)
			                           app_configuration.FPS = 0;

		                           text.setString(fmt::format(global_iser.translate_key("gui.main_menu.text.config_fps"), app_configuration.FPS));
		                         });
	main_buttons.emplace_front(
	    sf::Text(fmt::format(global_iser.translate_key("gui.main_menu.text.config_music_volume"), app_configuration.music_volume * 100.f), font_pixelish, 20),
	    volume_func("gui.main_menu.text.config_music_volume", app_configuration.music_volume, [&](auto) { music_volume_changed = true; }));
	main_buttons.emplace_front(
	    sf::Text(fmt::format(global_iser.translate_key("gui.main_menu.text.config_sound_effect_volume"), app_configuration.sound_effect_volume * 100.f),
	             font_pixelish, 20),
	    volume_func("gui.main_menu.text.config_sound_effect_volume", app_configuration.sound_effect_volume, [&](auto vol) {
		    selected_option_switch_sound->setVolume(output_volume(vol * .8));
		    selected_option_select_sound->setVolume(output_volume(vol * .8));
		    update_ready_sound->setVolume(output_volume(vol * .8));
		  }));
	main_buttons.emplace_front(sf::Text(fmt::format(global_iser.translate_key("gui.main_menu.text.config_default_firearm"),
	                                                firearm::properties().at(app_configuration.player_default_firearm).name),
	                                    font_pixelish, 20),
	                           [&, itr = firearm::properties().find(app_configuration.player_default_firearm) ](sf::Text & text) mutable {
		                           ++itr;
		                           if(itr == firearm::properties().end())
			                           itr = firearm::properties().begin();

		                           app_configuration.player_default_firearm = itr->first;
		                           text.setString(fmt::format(global_iser.translate_key("gui.main_menu.text.config_default_firearm"), itr->second.name));
		                         });

	selected = main_buttons.size() - 1;
}

void main_menu_screen::setup() {
	screen::setup();
}

int main_menu_screen::loop() {
	const auto & winsize = app.window.getSize();

	unsigned int buttid = 0;
	for(auto & button : main_buttons) {
		const auto & btnbds = button.first.getGlobalBounds();

		button.first.setPosition((winsize.x * (59.f / 60.f)) - btnbds.width,
		                         (winsize.y * (7.f / 8.f)) - (buttid + 1) * btnbds.height - (winsize.y * ((buttid * 1.f) / 90.f)));
		button.first.setFillColor((buttid == selected) ? sf::Color::Red : sf::Color::White);
		++buttid;
	}

	if(std::get<3>(update) && std::get<0>(update).valid()) {
		std::get<3>(update) = false;
		std::get<1>(update) = std::thread([&] {
			auto result = std::get<0>(update).get();
			std::cout << result.header["X-RateLimit-Remaining"] << " GitHub API accesses left\n";

			if(!(result.status_code >= 200 && result.status_code < 300))
				std::get<2>(update).setString(fmt::format(global_iser.translate_key("gui.main_menu.text.update_connection_fail"), result.status_code));
			else {
				json::value newest_update;
				json::parse(result.text, newest_update);

				auto new_version_s = newest_update["tag_name"].as<std::string>();
				new_version_s      = new_version_s.substr(new_version_s.find_first_of("0123456789"));

				if(version::Semver200_version(new_version_s) <= version::Semver200_version(BARBERSANDREBARBS_VERSION))
					std::get<2>(update).setString(global_iser.translate_key("gui.main_menu.text.update_none_found"));
				else {
					if(app_configuration.play_sounds)
						update_ready_sound->play();
					std::get<2>(update).setString(fmt::format(global_iser.translate_key("gui.main_menu.text.update_found"), new_version_s));

					main_buttons.emplace_back(
					    sf::Text(global_iser.translate_key("gui.main_menu.text.update"), font_swirly),
					    [&, url = newest_update["html_url"].as<std::string>() ](sf::Text &) {
						    if(!launch_browser(url)) {
							    main_buttons.clear();
							    main_buttons.emplace_front(sf::Text(global_iser.translate_key("gui.main_menu.text.back"), font_swirly),
							                               [&](sf::Text &) { set_default_menu_items(); });
							    main_buttons.emplace_front(sf::Text(global_iser.translate_key("gui.main_menu.text.update_browser_fail_0"), font_pixelish, 20),
							                               [&](sf::Text &) {});
							    main_buttons.emplace_front(sf::Text(url, font_pixelish, 20), [&](sf::Text & txt) { copy_to_clipboard(txt.getString()); });
							    main_buttons.emplace_front(sf::Text(global_iser.translate_key("gui.main_menu.text.update_browser_fail_1"), font_pixelish, 20),
							                               [&](sf::Text &) {});
							    selected = main_buttons.size() - 1;
						    }
						  });
				}
			}
		});
	}

	return 0;
}

int main_menu_screen::draw() {
	for(const auto & button : main_buttons)
		app.window.draw(button.first);

	try_drawings();
	if(joystick_drawing.first)
		app.window.draw(joystick_drawing.second);
	app.window.draw(keys_drawing);

	app.window.draw(std::get<2>(update));

	return 0;
}

int main_menu_screen::handle_event(const sf::Event & event) {
	if(int i = screen::handle_event(event))
		return i;

	switch(static_cast<int>(event.type)) {
		case sf::Event::MouseMoved: {
			unsigned int buttid = 0;
			for(const auto & button : main_buttons) {
				if(button.first.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
					if(app_configuration.play_sounds && selected != buttid)
						selected_option_switch_sound->play();
					selected = buttid;
					break;
				}
				++buttid;
			}
		} break;

		case sf::Event::MouseButtonPressed:
			if(event.mouseButton.button == sf::Mouse::Left)
				for(const auto & button : main_buttons)
					if(button.first.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						press_button();
						break;
					}
			break;

		case sf::Event::MouseWheelMoved:
			move_selection((event.mouseWheel.delta > 0) ? direction::up : direction::down);
			break;
		case sf::Event::KeyPressed:
			switch(static_cast<int>(event.key.code)) {
				case sf::Keyboard::Key::Home:
					move_selection(direction::up, true);
					break;
				case sf::Keyboard::Key::End:
					move_selection(direction::down, true);
					break;
				case sf::Keyboard::Key::Up:
					move_selection(direction::up);
					break;
				case sf::Keyboard::Key::Down:
					move_selection(direction::down);
					break;
				case sf::Keyboard::Key::Return:
				case sf::Keyboard::Key::Space:
					press_button();
					break;
			}
			break;

		case sf::Event::JoystickMoved:
			if(event.joystickMove.axis == X360_axis_mappings::LeftStickVertical || event.joystickMove.axis == X360_axis_mappings::DPadVertical) {
				if(event.joystickMove.position && (event.joystickMove.position >= 25 || event.joystickMove.position <= -25)) {
					if(joystick_up)
						break;
					joystick_up    = true;
					const int sign = event.joystickMove.position / std::abs(event.joystickMove.position);

					move_selection((sign == ((event.joystickMove.axis == X360_axis_mappings::LeftStickVertical) ? X360_axis_up_right_mappings::LeftStickVertical
					                                                                                            : X360_axis_up_right_mappings::DPadVertical))
					                   ? direction::up
					                   : direction::down);
				} else
					joystick_up = false;
			}
			break;

		case sf::Event::JoystickButtonPressed:
			if(event.joystickButton.button == X360_button_mappings::A)
				press_button();
			break;
	}

	return 0;
}

main_menu_screen::main_menu_screen(application & theapp)
      : screen(theapp), control_frames_counter(0), joystick_up(false), joystick_drawing(false, drawing("xbox", app.window.getSize())),
        keys_drawing("keyboard", app.window.getSize()),
        update(std::future<cpr::Response>(), std::thread(), sf::Text("", font_monospace, 10), app_configuration.use_network),
        selected_option_switch_sound(
            audiere::OpenSoundEffect(audio_device, (sound_root + "/main_menu/mouse_over.wav").c_str(), audiere::SoundEffectType::MULTIPLE)),
        selected_option_unchanged_sound(
            audiere::OpenSoundEffect(audio_device, (sound_root + "/main_menu/Alt_Fire_Switch.mp3").c_str(), audiere::SoundEffectType::MULTIPLE)),
        selected_option_select_sound(
            audiere::OpenSoundEffect(audio_device, (sound_root + "/main_menu/mouse_click.wav").c_str(), audiere::SoundEffectType::MULTIPLE)),
        update_ready_sound(audiere::OpenSoundEffect(audio_device, (sound_root + "/main_menu/update.wav").c_str(), audiere::SoundEffectType::SINGLE)) {
	selected_option_switch_sound->setVolume(output_volume(app_configuration.sound_effect_volume * .8));
	selected_option_unchanged_sound->setVolume(output_volume(app_configuration.sound_effect_volume * .8));
	selected_option_select_sound->setVolume(output_volume(app_configuration.sound_effect_volume * .8));
	update_ready_sound->setVolume(output_volume(app_configuration.sound_effect_volume * .8));

	if(app_configuration.use_network)
		std::get<0>(update) =
		    cpr::GetAsync(cpr::Url("https://api.github.com/repos/nabijaczleweli/BarbersAndRebarbs/releases/latest"), cpr::Parameters{{"anon", "true"}});

	keys_drawing.move(app.window.getSize().x / 4 - keys_drawing.size().x / 2, app.window.getSize().y / 2 - keys_drawing.size().y / 2);
	joystick_drawing.second.move(app.window.getSize().x / 4 - joystick_drawing.second.size().x / 2,
	                             app.window.getSize().y / 2 - joystick_drawing.second.size().y / 2);

	set_default_menu_items();
}

main_menu_screen::~main_menu_screen() {
	if(std::get<1>(update).joinable())
		std::get<1>(update).join();
}
