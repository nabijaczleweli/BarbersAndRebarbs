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


#include "drawing.hpp"
#include "../util/vector.hpp"
#include "../reference/container.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <iterator>
#include <cassert>
#include <fstream>


using namespace rapidjson;
using namespace std;
using namespace sf;


drawing::drawing(const string & asset_subpath, const Vector2f & s) : own_scale(1, 1) {
	ifstream drawing_file(assets_root + "/" + asset_subpath);
	IStreamWrapper drawing_file_wrap(drawing_file);

	Document doc;
	doc.ParseStream(drawing_file_wrap);

	loaded_size = {doc["size"]["x"].GetDouble(), doc["size"]["y"].GetDouble()};

	auto && elements = doc["elements"];
	for(auto itr = elements.Begin(); itr != elements.End(); ++itr) {
		auto && element = *itr;

		auto && type = element["type"];
		auto && data = element["data"];

		if(type == "line") {
			line lne;
			lne[0].position.x += data[0]["x"].GetDouble() + s.x;
			lne[0].position.y += data[0]["y"].GetDouble() + s.y;
			lne[1].position.x += data[1]["x"].GetDouble() + s.x;
			lne[1].position.y += data[1]["y"].GetDouble() + s.y;
			lines.emplace_back(lne);
		} else if(type == "triangle") {
			triangle trgl;
			trgl[0].position.x += data[0]["x"].GetDouble() + s.x;
			trgl[0].position.y += data[0]["y"].GetDouble() + s.y;
			trgl[1].position.x += data[1]["x"].GetDouble() + s.x;
			trgl[1].position.y += data[1]["y"].GetDouble() + s.y;
			trgl[2].position.x += data[2]["x"].GetDouble() + s.x;
			trgl[2].position.y += data[2]["y"].GetDouble() + s.y;
			triangles.emplace_back(trgl);
		} else if(type == "bezier_curve") {
			Vector2f start_point;
			Vector2f control_point;
			Vector2f end_point;

			start_point.x   = data["start"]["x"].GetDouble() + s.x;
			start_point.y   = data["start"]["y"].GetDouble() + s.y;
			control_point.x = data["control"]["x"].GetDouble() + s.x;
			control_point.y = data["control"]["y"].GetDouble() + s.y;
			end_point.x     = data["end"]["x"].GetDouble() + s.x;
			end_point.y     = data["end"]["y"].GetDouble() + s.y;

			curves.emplace_back(start_point, control_point, end_point);
		}
	}
}

void drawing::draw(RenderTarget & target, RenderStates states) const {
	for(auto && line : lines)
		target.draw(line.data(), line.size(), PrimitiveType::Lines);
	for(auto && triangle : triangles)
		target.draw(triangle.data(), triangle.size(), PrimitiveType::Triangles);
	for(auto && curve : curves)
		curve.draw(target, states);
}

void drawing::move(float x, float y) {
	Vector2f offset(x, y);

	for(auto && line : lines)
		for(auto && point : line)
			point.position += offset;

	for(auto && triangle : triangles)
		for(auto && point : triangle)
			point.position += offset;

	for(auto && curve : curves)
		curve.move(x, y);
}

void drawing::scale_size(Vector2f factor) {
	factor.x /= 1600.f;
	factor.y /= 900.f;
	own_scale = own_scale * factor;

	for(auto && line : lines)
		for(auto && point : line)
			point.position = point.position * factor;

	for(auto && triangle : triangles)
		for(auto && point : triangle)
			point.position = point.position * factor;

	for(auto && curve : curves)
		curve.scale(factor);
}

Vector2f drawing::size() const {
	return static_cast<Vector2f>(loaded_size * own_scale);
}
