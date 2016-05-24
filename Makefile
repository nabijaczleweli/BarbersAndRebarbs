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


LDDLLS := $(OS_LD_LIBS) audiere cpp-localiser cpp-nbt whereami++ seed11
LDAR := $(LNCXXAR) -L$(BLDDIR)Cpp-NBT -L$(BLDDIR)seed11 -L$(BLDDIR)cpp-localiser -L$(BLDDIR)whereami-cpp -L$(BLDDIR)audiere/lib $(foreach dll,$(LDDLLS),-l$(dll))
INCAR := $(foreach l,$(foreach l,audiere cereal cimpoler-meta cpp-localiser Cpp-NBT seed11 whereami-cpp,$(l)/include) jsonpp,-isystemext/$(l))
VERAR := $(foreach l,CEREAL CIMPOLER_META CPP_LOCALISER CPP_NBT JSONPP SEED11 WHEREAMI_CPP,-D$(l)_VERSION='$($(l)_VERSION)')
SOURCES := $(sort $(wildcard src/*.cpp src/**/*.cpp src/**/**/*.cpp src/**/**/**/*.cpp))
HEADERS := $(sort $(wildcard src/*.hpp src/**/*.hpp src/**/**/*.hpp src/**/**/**/*.hpp))

.PHONY : all clean assets exe audiere cpp-localiser cpp-nbt seed11 whereami-cpp


all : assets audiere cpp-localiser cpp-nbt seed11 whereami-cpp exe

clean :
	rm -rf $(OUTDIR)

assets :
	@rm -rf $(OUTDIR)assets
	@cp -r $(ASSETDIR) $(OUTDIR)

exe : audiere cpp-nbt seed11 whereami-cpp $(OUTDIR)BarbersAndRebarbs$(EXE)
audiere : $(BLDDIR)audiere/lib/libaudiere$(DLL)
cpp-localiser : $(BLDDIR)cpp-localiser/libcpp-localiser$(ARCH)
cpp-nbt : $(BLDDIR)Cpp-NBT/libcpp-nbt$(ARCH)
seed11 : $(BLDDIR)seed11/libseed11$(ARCH)
whereami-cpp : $(BLDDIR)whereami-cpp/libwhereami++$(ARCH)


$(OUTDIR)BarbersAndRebarbs$(EXE) : $(subst $(SRCDIR),$(OBJDIR),$(subst .cpp,$(OBJ),$(SOURCES)))
	$(CXX) -Wl,-rpath=$(BLDDIR)audiere/lib,-rpath=. $(CXXAR) -o$@ $(subst $(SRCDIR),$(OBJDIR),$^) $(PIC) $(LDAR) $(shell grep '<SFML/' $(HEADERS) $(SOURCES) | sed -r 's:.*#include <SFML/(.*).hpp>:-lsfml-\1$(SFML_LINK_SUFF):' | tr '[:upper:]' '[:lower:]' | sort | uniq)

$(BLDDIR)audiere/lib/libaudiere$(DLL) : ext/audiere/CMakeLists.txt
	@mkdir -p $(abspath $(dir $@)../build)
	# FLAC doesn't seem to work on Travis by default so v0v
	cd $(abspath $(dir $@)../build) && $(INCCMAKEAR) $(LNCMAKEAR) $(CMAKE) -DUSE_FLAC=OFF -DCMAKE_INSTALL_PREFIX:PATH="$(abspath $(dir $@)..)" $(abspath $(dir $^)) -GNinja
	cd $(abspath $(dir $@)../build) && $(NINJA) install
	$(if $(OS) | grep Windows_NT,cp $@ $(OUTDIR))

$(BLDDIR)Cpp-NBT/libcpp-nbt$(ARCH) : ext/Cpp-NBT/Makefile
	$(MAKE) -C$(dir $^) BUILD=$(abspath $(dir $@)) stlib

$(BLDDIR)cpp-localiser/libcpp-localiser$(ARCH) : ext/cpp-localiser/Makefile
	$(MAKE) -C$(dir $^) BUILD=$(abspath $(dir $@)) stlib

$(BLDDIR)seed11/libseed11$(ARCH) : $(foreach src,seed11_system_agnostic seed11_$(SEED11_SYSTEM_TYPE) deterministic_unsafe_seed_device,$(BLDDIR)seed11/obj/$(src).o)
	$(AR) crs $@ $^

$(BLDDIR)whereami-cpp/libwhereami++$(ARCH) : ext/whereami-cpp/Makefile
	$(MAKE) -C$(dir $^) BUILD=$(abspath $(dir $@)) stlib


$(OBJDIR)%$(OBJ) : $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) $(INCAR) $(VERAR) -c -o$@ $^

$(BLDDIR)seed11/obj/%.o : ext/seed11/src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) -isystemext/seed11/include -c -o$@ $^
