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


SUBSYSTEMS_SFML := system window graphics
LDDLLS := audiere $(foreach subsystem,$(SUBSYSTEMS_SFML),sfml-$(subsystem)-2) cpp-nbt
LDAR := -L$(OUTDIR)/Cpp-NBT $(foreach dll,$(LDDLLS),-l$(dll))
SOURCES := $(sort $(wildcard src/**/**/**/*.cpp src/**/**/**/**/*.cpp src/**/**/**/**/**/*.cpp src/**/**/**/**/**/**/*.cpp))


.PHONY : all clean assets exe cpp-nbt


all : assets cpp-nbt exe

clean :
	rm -rf $(OUTDIR)

assets :
	@cp -r $(ASSETDIR) $(OUTDIR)

exe : $(OUTDIR)BarbersAndRebarbs$(EXE)
cpp-nbt : $(OUTDIR)/Cpp-NBT/libcpp-nbt$(ARCH)


$(OUTDIR)BarbersAndRebarbs$(EXE) : $(subst $(SRCDIR),$(OBJDIR),$(subst .cpp,$(OBJ),$(SOURCES)))
	$(CXX) $(CPPAR) -o$@ $(subst $(SRCDIR),$(OBJDIR),$^) $(PIC) $(LDAR)

$(OUTDIR)/Cpp-NBT/libcpp-nbt$(ARCH) : ext/Cpp-NBT/Makefile
	$(MAKE) -C$(dir $^) BUILD=$(abspath $(dir $@))


$(OBJDIR)%$(OBJ) : $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPAR) -Iext/cereal/include -Iext/Cpp-NBT/include -DCEREAL_VERSION='$(CEREAL_VERSION)' -DCPP_NBT_VERSION='$(CPP_NBT_VERSION)' -c -o$@ $^
