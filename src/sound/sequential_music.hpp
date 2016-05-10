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


#pragma once


#include "audiere.h"
#include <memory>


class sequential_music : public audiere::RefImplementation<audiere::OutputStream> {
protected:
	sequential_music();

private:
	bool loop = true;
	audiere::OutputStreamPtr output_stream;
	unsigned int music_id = 0;

public:
	unsigned int max_id = 0;
	std::function<std::string(unsigned int)> getter_algo;

	sequential_music(unsigned int max_id, std::function<std::string(unsigned int)> getter_algo);
	sequential_music(unsigned int max_id, const std::string & filename);
	virtual ~sequential_music();

	virtual void tick();
	void go_to_next();


	ADR_METHOD(void) play();
	ADR_METHOD(void) stop();
	ADR_METHOD(bool) isPlaying();
	ADR_METHOD(void) reset();
	ADR_METHOD(void) setRepeat(bool repeat);
	ADR_METHOD(bool) getRepeat();
	ADR_METHOD(void) setVolume(float volume);
	ADR_METHOD(float) getVolume();
	ADR_METHOD(void) setPan(float pan);
	ADR_METHOD(float) getPan();
	ADR_METHOD(void) setPitchShift(float shift);
	ADR_METHOD(float) getPitchShift();
	ADR_METHOD(bool) isSeekable();
	ADR_METHOD(int) getLength();
	ADR_METHOD(void) setPosition(int position);
	ADR_METHOD(int) getPosition();
};

class quiet_music : public sequential_music {
public:
	quiet_music() : sequential_music() {}

	inline virtual void tick() {}
	inline ADR_METHOD(void) ref() {}
	inline ADR_METHOD(void) unref() {}
	inline ADR_METHOD(void) play() {}
	inline ADR_METHOD(void) stop() {}
	inline ADR_METHOD(bool) isPlaying() { return false; }
	inline ADR_METHOD(void) reset() {}
	inline ADR_METHOD(void) setRepeat(bool) {}
	inline ADR_METHOD(bool) getRepeat() { return false; }
	inline ADR_METHOD(void) setVolume(float) {}
	inline ADR_METHOD(float) getVolume() { return 0.f; }
	inline ADR_METHOD(void) setPan(float) {}
	inline ADR_METHOD(float) getPan() { return 0.f; }
	inline ADR_METHOD(void) setPitchShift(float) {}
	inline ADR_METHOD(float) getPitchShift() { return 1.f; }
	inline ADR_METHOD(bool) isSeekable() { return false; }
	inline ADR_METHOD(int) getLength() { return 0; }
	inline ADR_METHOD(void) setPosition(int) {}
	inline ADR_METHOD(int) getPosition() { return 0; }
};


typedef audiere::RefPtr<sequential_music> SequentialMusicPtr;
extern const std::unique_ptr<quiet_music> silent_music;
