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


#include <audiere.h>
#include <memory>


class sequential_music : public audiere::RefImplementation<audiere::OutputStream> {
private:
	bool loop;
	audiere::OutputStreamPtr output_stream;
	unsigned int music_id;
	unsigned int max_id;
	std::function<std::string(unsigned int)> getter_algo;

public:
	sequential_music();
	sequential_music(unsigned int max_id, std::function<std::string(unsigned int)> getter_algo);
	sequential_music(unsigned int max_id, const std::string & filename);
	virtual ~sequential_music() = default;

	virtual void tick();
	void go_to_next();


	ADR_METHOD(void) play() override;
	ADR_METHOD(void) stop() override;
	ADR_METHOD(bool) isPlaying() override;
	ADR_METHOD(void) reset() override;
	ADR_METHOD(void) setRepeat(bool repeat) override;
	ADR_METHOD(bool) getRepeat() override;
	ADR_METHOD(void) setVolume(float volume) override;
	ADR_METHOD(float) getVolume() override;
	ADR_METHOD(void) setPan(float pan) override;
	ADR_METHOD(float) getPan() override;
	ADR_METHOD(void) setPitchShift(float shift) override;
	ADR_METHOD(float) getPitchShift() override;
	ADR_METHOD(bool) isSeekable() override;
	ADR_METHOD(int) getLength() override;
	ADR_METHOD(void) setPosition(int position) override;
	ADR_METHOD(int) getPosition() override;
};
