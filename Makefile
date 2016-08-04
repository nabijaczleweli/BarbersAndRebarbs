# The MIT License (MIT)

# Copyright (c) 2014 nabijaczleweli

# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


include configMakefile


LDDLLS := $(OS_LD_LIBS) audiere cpp-localiser cpr fmt whereami++ seed11 semver semver-utils zstd curl
LDAR := $(LNCXXAR) $(foreach l,audiere/lib cpp-localiser cpr/lib fmt whereami-cpp seed11 semver semver-utils zstd,-L$(BLDDIR)$(l)) $(foreach dll,$(LDDLLS),-l$(dll))
INCAR := $(foreach l,$(foreach l,audiere cereal cimpoler-meta cpp-localiser seed11 whereami-cpp,$(l)/include) jsonpp,-isystemext/$(l)) $(foreach l,cpr fmt semver semver-utils zstd,-isystem$(BLDDIR)$(l)/include)
VERAR := $(foreach l,BARBERSANDREBARBS CEREAL CIMPOLER_META CPP_LOCALISER CPR FMT JSONPP SEED11 SEMVER SEMVER_UTILS WHEREAMI_CPP,-D$(l)_VERSION='$($(l)_VERSION)')
SOURCES := $(sort $(wildcard src/*.cpp src/**/*.cpp src/**/**/*.cpp src/**/**/**/*.cpp))
HEADERS := $(sort $(wildcard src/*.hpp src/**/*.hpp src/**/**/*.hpp src/**/**/**/*.hpp))

.PHONY : all clean assets exe audiere cpp-localiser cpr fmt seed11 semver semver-utils zstd whereami-cpp


all : assets audiere cpp-localiser cpr fmt seed11 semver semver-utils whereami-cpp zstd exe

clean :
	rm -rf $(OUTDIR)

assets :
	@rm -rf $(OUTDIR)assets
	@cp -r $(ASSETDIR) $(OUTDIR)

exe : audiere cpp-localiser cpr seed11 fmt seed11 semver-utils whereami-cpp zstd $(OUTDIR)BarbersAndRebarbs$(EXE)
audiere : $(BLDDIR)audiere/lib/libaudiere$(DLL)
cpp-localiser : $(BLDDIR)cpp-localiser/libcpp-localiser$(ARCH)
cpr : $(BLDDIR)cpr/lib/libcpr$(ARCH) $(BLDDIR)cpr/include/cpr/cpr.h
fmt : $(BLDDIR)fmt/libfmt$(ARCH) $(BLDDIR)fmt/include/fmt/format.h
seed11 : $(BLDDIR)seed11/libseed11$(ARCH)
semver : $(BLDDIR)semver/libsemver$(ARCH) $(BLDDIR)semver/include/semver/semver200.h
semver-utils : $(BLDDIR)semver-utils/libsemver-utils$(ARCH) $(BLDDIR)semver-utils/include/semver-utils/version.hpp
whereami-cpp : $(BLDDIR)whereami-cpp/libwhereami++$(ARCH)
zstd : $(BLDDIR)zstd/libzstd$(ARCH) $(BLDDIR)zstd/include/zstd/common/zstd.h


$(OUTDIR)BarbersAndRebarbs$(EXE) : $(subst $(SRCDIR),$(OBJDIR),$(subst .cpp,$(OBJ),$(SOURCES)))
	$(CXX) -Wl,-rpath=$(BLDDIR)audiere/lib,-rpath=. $(CXXAR) -o$@ $(subst $(SRCDIR),$(OBJDIR),$^) $(PIC) $(LDAR) $(shell grep '<SFML/' $(HEADERS) $(SOURCES) | sed -r 's:.*#include <SFML/(.*).hpp>:-lsfml-\1$(SFML_LINK_SUFF):' | tr '[:upper:]' '[:lower:]' | sort | uniq)

$(BLDDIR)audiere/lib/libaudiere$(DLL) : ext/audiere/CMakeLists.txt
	@mkdir -p $(abspath $(dir $@)../build)
	# FLAC doesn't seem to work on Travis by default so v0v
	cd $(abspath $(dir $@)../build) && $(INCCMAKEAR) $(LNCMAKEAR) $(CMAKE) -DUSE_FLAC=OFF -DCMAKE_INSTALL_PREFIX:PATH="$(abspath $(dir $@)..)" $(abspath $(dir $^)) -GNinja
	cd $(abspath $(dir $@)../build) && $(NINJA) install
	$(if $(OS) | grep Windows_NT,cp $@ $(OUTDIR))

$(BLDDIR)cpp-localiser/libcpp-localiser$(ARCH) : ext/cpp-localiser/Makefile
	$(MAKE) -C$(dir $^) BUILD=$(abspath $(dir $@)) stlib

$(BLDDIR)cpr/lib/libcpr$(ARCH) : ext/cpr/CMakeLists.txt
	@mkdir -p $(abspath $(dir $@)..)
	cd $(abspath $(dir $@)..) && CXXFLAGS="$(INCCXXAR) -DCURL_STATICLIB" $(LNCMAKEAR) $(CMAKE) -DUSE_SYSTEM_CURL=ON -DBUILD_CPR_TESTS=OFF $(abspath $(dir $^)) -GNinja
	cd $(abspath $(dir $@)..) && $(NINJA)

$(BLDDIR)cpr/include/cpr/cpr.h : ext/cpr/include/cpr.h
	@mkdir -p $(abspath $(dir $@)..)
	cp -r $(dir $^) $(dir $@)

$(BLDDIR)seed11/libseed11$(ARCH) : $(foreach src,seed11_system_agnostic seed11_$(SEED11_SYSTEM_TYPE) deterministic_unsafe_seed_device,$(BLDDIR)seed11/obj/$(src)$(OBJ))
	$(AR) crs $@ $^

$(BLDDIR)fmt/libfmt$(ARCH) : $(patsubst ext/fmt/fmt/%.cc,$(BLDDIR)fmt/obj/%$(OBJ),$(wildcard ext/fmt/fmt/*.cc))
	$(AR) crs $@ $^

$(BLDDIR)fmt/include/fmt/format.h : $(wildcard ext/fmt/fmt/*.h)
	@mkdir -p $(dir $@)
	cp $^ $(dir $@)

$(BLDDIR)semver/libsemver$(ARCH) : $(patsubst ext/semver/src/%.cpp,$(BLDDIR)semver/obj/%$(OBJ),$(wildcard ext/semver/src/*.cpp))
	$(AR) crs $@ $^

$(BLDDIR)semver/include/semver/semver200.h : $(wildcard ext/semver/include/*.h ext/semver/include/*.inl)
	@mkdir -p $(dir $@)
	cp $^ $(dir $@)

$(BLDDIR)semver-utils/libsemver-utils$(ARCH) : $(BLDDIR)semver-utils/obj/version$(OBJ)
	$(AR) crs $@ $^

$(BLDDIR)semver-utils/include/semver-utils/version.hpp : ext/semver-utils/src/libsemver/c++/version.hpp
	@mkdir -p $(dir $@)
	cp $^ $@

$(BLDDIR)semver-utils/src/version.cpp : ext/semver-utils/src/libsemver/c++/version.cpp
	@mkdir -p $(dir $@)
	sed -e 's:#include "../gettext_defs.h":#define _(S) S:' -e 's:#include "version.hpp":#include "../include/semver-utils/version.hpp":' $^ > $@

$(BLDDIR)whereami-cpp/libwhereami++$(ARCH) : ext/whereami-cpp/Makefile
	$(MAKE) -C$(dir $^) BUILD=$(abspath $(dir $@)) stlib

$(BLDDIR)zstd/libzstd$(ARCH) : $(subst ext/zstd/lib,$(BLDDIR)zstd/obj,$(subst .c,$(OBJ),$(wildcard ext/zstd/lib/common/*.c ext/zstd/lib/compress/*.c ext/zstd/lib/decompress/*.c)))
	@mkdir -p $(dir $@)
	$(AR) crs $@ $^

$(BLDDIR)zstd/include/zstd/common/zstd.h : $(wildcard ext/zstd/lib/common/*.h ext/zstd/lib/compress/*.h ext/zstd/lib/decompress/*.h)
	@mkdir -p $(foreach incfile,$(subst ext/zstd/lib,$(BLDDIR)zstd/include/zstd,$^),$(abspath $(dir $(incfile))))
	$(foreach incfile,$^,cp $(incfile) $(subst ext/zstd/lib,$(BLDDIR)zstd/include/zstd,$(incfile));)


$(OBJDIR)%$(OBJ) : $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) $(INCAR) $(VERAR) -DZSTD_STATIC_LINKING_ONLY -c -o$@ $^

$(BLDDIR)fmt/obj/%$(OBJ) : ext/fmt/fmt/%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) -Iext/fmt -c -o$@ $^

$(BLDDIR)semver/obj/%$(OBJ) : ext/semver/src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) -Iext/semver/include -c -o$@ $^

$(BLDDIR)semver-utils/obj/%$(OBJ) : $(BLDDIR)semver-utils/src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) -DSEMVER_GETTEXT_DEFS_H -D_LIBGETTEXT_H -Wno-unknown-pragmas -c -o$@ $^

$(BLDDIR)seed11/obj/%$(OBJ) : ext/seed11/src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) -isystemext/seed11/include -c -o$@ $^

$(BLDDIR)zstd/obj/%$(OBJ) : ext/zstd/lib/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CCAR) -Iext/zstd/lib/common -DZSTD_STATIC_LINKING_ONLY -c -o$@ $^
