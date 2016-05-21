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
#include "../reference/container.hpp"
#include "../util/vector.hpp"
#include <fstream>
#include <iterator>
#include <jsonpp/parser.hpp>


using namespace std;
using namespace sf;


drawing::drawing(const string & model_name, const Vector2f & window_size) : own_scale(1, 1) {
	ifstream drawing_file(drawing_root + "/" + model_name + ".json");
	json::value doc;
	json::parse(drawing_file, doc);

	origin_size = {doc["origin_size"]["x"].as<float>(), doc["origin_size"]["y"].as<float>()};
	loaded_size = {doc["size"]["x"].as<float>(), doc["size"]["y"].as<float>()};

	for(auto && element : doc["elements"].as<json::array>()) {
		auto && type = element["type"].as<string>();

		if(type == "line") {
			auto && data = element["data"].as<json::array>();
			line lne;
			lne[0].position.x = data[0]["x"].as<double>();
			lne[0].position.y = data[0]["y"].as<double>();
			lne[1].position.x = data[1]["x"].as<double>();
			lne[1].position.y = data[1]["y"].as<double>();
			lines.emplace_back(lne);
		} else if(type == "triangle") {
			auto && data = element["data"].as<json::array>();
			triangle trgl;
			trgl[0].position.x = data[0]["x"].as<double>();
			trgl[0].position.y = data[0]["y"].as<double>();
			trgl[1].position.x = data[1]["x"].as<double>();
			trgl[1].position.y = data[1]["y"].as<double>();
			trgl[2].position.x = data[2]["x"].as<double>();
			trgl[2].position.y = data[2]["y"].as<double>();
			triangles.emplace_back(trgl);
		} else if(type == "rectangle") {
			auto && data = element["data"].as<json::array>();
			rectangle rctgl;
			rctgl[0].position.x = data[0]["x"].as<double>();
			rctgl[0].position.y = data[0]["y"].as<double>();
			rctgl[1].position.x = data[1]["x"].as<double>();
			rctgl[1].position.y = data[1]["y"].as<double>();
			rctgl[2].position.x = data[2]["x"].as<double>();
			rctgl[2].position.y = data[2]["y"].as<double>();
			rctgl[3].position.x = data[3]["x"].as<double>();
			rctgl[3].position.y = data[3]["y"].as<double>();
			rctgl[4]            = rctgl[0];
			rectangles.emplace_back(rctgl);
		} else if(type == "bezier_curve") {
			auto && data = element["data"];
			Vector2f start_point;
			Vector2f control_point;
			Vector2f end_point;

			start_point.x   = data["start"]["x"].as<double>();
			start_point.y   = data["start"]["y"].as<double>();
			control_point.x = data["control"]["x"].as<double>();
			control_point.y = data["control"]["y"].as<double>();
			end_point.x     = data["end"]["x"].as<double>();
			end_point.y     = data["end"]["y"].as<double>();

			curves.emplace_back(start_point, control_point, end_point);
		}
	}

	scale_size(window_size);
}

void drawing::draw(RenderTarget & target, RenderStates states) const {
	for(auto && line : lines)
		target.draw(line.data(), line.size(), PrimitiveType::Lines);
	for(auto && triangle : triangles)
		target.draw(triangle.data(), triangle.size(), PrimitiveType::Triangles);
	for(auto && rectangle : rectangles)
		target.draw(rectangle.data(), rectangle.size(), PrimitiveType::LinesStrip);
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

	for(auto && rectangle : rectangles)
		for(auto && point : rectangle)
			point.position += offset;

	for(auto && curve : curves)
		curve.move(x, y);
}

void drawing::scale_size(Vector2f factor) {
	factor    = factor / origin_size;
	own_scale = own_scale * factor;

	for(auto && line : lines)
		for(auto && point : line)
			point.position = point.position * factor;

	for(auto && triangle : triangles)
		for(auto && point : triangle)
			point.position = point.position * factor;

	for(auto && rectangle : rectangles)
		for(auto && point : rectangle)
			point.position = point.position * factor;

	for(auto && curve : curves)
		curve.scale(factor);
}

Vector2f drawing::size() const {
	return loaded_size * own_scale;
}
