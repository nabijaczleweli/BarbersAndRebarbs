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


LDDLLS := audiere cpp-nbt whereami++ seed11
LDAR := $(LNCXXAR) -L$(BLDDIR)Cpp-NBT -L$(BLDDIR)seed11 -L$(BLDDIR)whereami-cpp $(foreach dll,$(LDDLLS),-l$(dll))
SOURCES := $(sort $(wildcard src/*.cpp src/**/*.cpp src/**/**/*.cpp src/**/**/**/*.cpp))
HEADERS := $(sort $(wildcard src/*.hpp src/**/*.hpp src/**/**/*.hpp src/**/**/**/*.hpp))


.PHONY : all clean assets exe seed11 cpp-nbt whereami-cpp sfml-deplist


all : assets seed11 cpp-nbt whereami-cpp sfml-deplist exe

clean :
	rm -rf $(OUTDIR)

assets :
	@cp -r $(ASSETDIR) $(OUTDIR)

exe : $(OUTDIR)BarbersAndRebarbs$(EXE)
seed11 : $(BLDDIR)seed11/libseed11$(ARCH)
cpp-nbt : $(BLDDIR)Cpp-NBT/libcpp-nbt$(ARCH)
whereami-cpp : $(BLDDIR)whereami-cpp/libwhereami++$(ARCH)
sfml-deplist : $(BLDDIR)sfml-modules


$(OUTDIR)BarbersAndRebarbs$(EXE) : $(subst $(SRCDIR),$(OBJDIR),$(subst .cpp,$(OBJ),$(SOURCES)))
	$(CXX) $(CXXAR) -o$@ $(subst $(SRCDIR),$(OBJDIR),$^) $(PIC) $(LDAR) @$(BLDDIR)sfml-modules

$(BLDDIR)seed11/libseed11$(ARCH) : $(foreach src,seed11_system_agnostic seed11_$(SEED11_SYSTEM_TYPE) deterministic_unsafe_seed_device,$(BLDDIR)seed11/obj/$(src).o)
	$(AR) crs $@ $^

$(BLDDIR)Cpp-NBT/libcpp-nbt$(ARCH) : ext/Cpp-NBT/Makefile
	$(MAKE) -C$(dir $^) BUILD=$(abspath $(dir $@)) stlib

$(BLDDIR)whereami-cpp/libwhereami++$(ARCH) : ext/whereami-cpp/Makefile
	$(MAKE) -C$(dir $^) BUILD=$(abspath $(dir $@)) stlib

$(BLDDIR)sfml-modules : $(HEADERS) $(SOURCES)
	@mkdir -p $(dir $@)
	grep "<SFML/" $^ | sed -r "s:.*#include <SFML/(.*).hpp>:-lsfml-\\1$(SFML_LINK_SUFF):" | tr '[:upper:]' '[:lower:]' | sort | uniq > $@


$(OBJDIR)%$(OBJ) : $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) -Iext/cereal/include -Iext/cimpoler-meta/include -Iext/Cpp-NBT/include -Iext/seed11/include -Iext/whereami-cpp/include -DCEREAL_VERSION='$(CEREAL_VERSION)' -DCIMPOLER_META_VERSION='$(CIMPOLER_META_VERSION)' -DCPP_NBT_VERSION='$(CPP_NBT_VERSION)' -DSEED11_VERSION='$(SEED11_VERSION)' -DWHEREAMI_CPP_VERSION='$(WHEREAMI_CPP_VERSION)' -c -o$@ $^

$(BLDDIR)seed11/obj/%.o : ext/seed11/src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) -isystemext/seed11/include -c -o$@ $^
