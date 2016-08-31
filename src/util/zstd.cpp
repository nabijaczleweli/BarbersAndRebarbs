// The MIT License (MIT)

// Copyright (c) 2016 nabijaczleweli

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


#include "zstd.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <memory>
#include <zstd/zstd.h>


static const char * compress_string_to_file(const char * filename, const char * data, std::size_t data_len);


const char * compress_string_to_file(const std::string & filename, const std::string & data) {
	return compress_string_to_file(filename.c_str(), data.c_str(), data.size());
}

const char * compress_string_to_file(const std::string & filename, const char * data) {
	return compress_string_to_file(filename.c_str(), data, std::strlen(data));
}

const char * compress_string_to_file(const char * filename, const std::string & data) {
	return compress_string_to_file(filename, data.c_str(), data.size());
}

const char * compress_string_to_file(const char * filename, const char * data) {
	return compress_string_to_file(filename, data, std::strlen(data));
}

std::tuple<std::string, bool, const char *> decompress_file_to_string(const std::string & filename) {
	return decompress_file_to_string(filename.c_str());
}


static const char * compress_string_to_file(const char * filename, const char * data, std::size_t data_len) {
	auto out_c             = std::make_unique<std::uint8_t[]>(ZSTD_compressBound(data_len));
	const auto comp_result = ZSTD_compress(out_c.get(), ZSTD_compressBound(data_len), data, data_len, ZSTD_maxCLevel());
	if(ZSTD_isError(comp_result)) {
		return ZSTD_getErrorName(comp_result);
	} else {
		std::uint64_t raw_size        = data_len;
		std::uint64_t compressed_size = comp_result;
		std::ofstream(filename, std::ios::binary)
		    .write(reinterpret_cast<const char *>(&raw_size), sizeof(raw_size))
		    .write(reinterpret_cast<const char *>(&compressed_size), sizeof(compressed_size))
		    .write(reinterpret_cast<const char *>(out_c.get()), compressed_size);

		return nullptr;
	}
}

std::tuple<std::string, bool, const char *> decompress_file_to_string(const char * filename) {
	std::uint64_t raw_size;
	std::uint64_t compressed_size;
	std::ifstream save_file(filename, std::ios::binary);
	if(!save_file.is_open()) {
		return std::make_tuple("", false, nullptr);
	}

	save_file.read(reinterpret_cast<char *>(&raw_size), sizeof(raw_size)).read(reinterpret_cast<char *>(&compressed_size), sizeof(compressed_size));
	auto in_c = std::make_unique<std::uint8_t[]>(compressed_size);
	save_file.read(reinterpret_cast<char *>(in_c.get()), compressed_size);

	std::string in(raw_size, '\0');
	const auto result = ZSTD_decompress(&in[0], in.size(), in_c.get(), compressed_size);
	if(ZSTD_isError(result)) {
		return std::make_tuple(in, true, ZSTD_getErrorName(result));
	} else {
		return std::make_tuple(in, true, nullptr);
	}
}
