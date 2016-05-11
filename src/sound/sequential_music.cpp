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


#include "../reference/container.hpp"
#include "sequential_music.hpp"
#include <algorithm>


using namespace audiere;
using namespace std;


sequential_music::sequential_music() : getter_algo([](auto) { return ""; }) {}
sequential_music::sequential_music(unsigned int seq_len, function<string(unsigned int)> getter)
      : loop(true), output_stream(nullptr), music_id(0), max_id(seq_len - 1), getter_algo(getter) {
	if(seq_len)
		output_stream = OpenSound(audio_device, getter(0).c_str(), true);
}
sequential_music::sequential_music(unsigned int maximal_id, const string & filename) : sequential_music(maximal_id, [=](unsigned int) { return filename; }) {}

void sequential_music::tick() {
	if(!output_stream || (output_stream && isPlaying()))
		return;
	go_to_next();
}

void sequential_music::go_to_next() {
	stop();

	++music_id;
	music_id = min(music_id, max_id);
	if(music_id == max_id) {
		if(getRepeat())
			music_id = 0;
		else {
			output_stream = nullptr;
			return;
		}
	}

	output_stream = OpenSound(audio_device, getter_algo(music_id).c_str(), true);
	play();
}

void sequential_music::play() {
	if(output_stream)
		output_stream->play();
}

void sequential_music::stop() {
	if(output_stream)
		output_stream->stop();
}

bool sequential_music::isPlaying() {
	return output_stream && output_stream->isPlaying();
}

void sequential_music::reset() {
	if(output_stream)
		output_stream->reset();
}

void sequential_music::setRepeat(bool repeat) {
	loop = repeat;
}

bool sequential_music::getRepeat() {
	return loop;
}

void sequential_music::setVolume(float volume) {
	if(output_stream)
		output_stream->setVolume(volume);
}

float sequential_music::getVolume() {
	return output_stream ? output_stream->getVolume() : 0;
}

void sequential_music::setPan(float pan) {
	if(output_stream)
		output_stream->setPan(pan);
}

float sequential_music::getPan() {
	return output_stream ? output_stream->getPan() : 0;
}

void sequential_music::setPitchShift(float shift) {
	if(output_stream)
		output_stream->setPitchShift(shift);
}

float sequential_music::getPitchShift() {
	return output_stream ? output_stream->getPitchShift() : 0;
}

bool sequential_music::isSeekable() {
	return output_stream && output_stream->isSeekable();
}

int sequential_music::getLength() {
	return output_stream ? output_stream->getLength() : 0;
}

void sequential_music::setPosition(int position) {
	if(output_stream)
		output_stream->setPosition(position);
}

int sequential_music::getPosition() {
	return output_stream ? output_stream->getPosition() : 0;
}
