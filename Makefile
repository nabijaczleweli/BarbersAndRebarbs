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
CUSTOM_DLLS := p:/C++/Utilities/Cpp-NBT/out/cpp-nbt$(DLL)
LDDLLS := audiere $(foreach subsystem,$(SUBSYSTEMS_SFML),sfml-$(subsystem)-2) cpp-nbt
LDAR := -fpic -L"p:/C++/Utilities/Cpponfiguration/out" -L"p:/C++/Utilities/Cpp-NBT/out" $(foreach dll,$(LDDLLS),-l$(dll))
SOURCES := $(sort $(wildcard src/**/**/**/*.cpp src/**/**/**/**/*.cpp src/**/**/**/**/**/*.cpp src/**/**/**/**/**/**/*.cpp))


.PHONY : clean all release git


all : $(subst $(SRCDIR),$(OBJDIR),$(subst .cpp,$(OBJ),$(SOURCES)))
	$(CXX) $(CPPAR) -o$(OUTDIR)BarbersAndRebarbs$(EXE) $(subst $(SRCDIR),$(OBJDIR),$^) $(LDAR)
	@cp -r $(ASSETDIR) $(OUTDIR)

clean :
	rm -rf $(OUTDIR) $(RELEASEDIR)

release : clean all
	@mkdir $(RELEASEDIR)
	cp $(OUTDIR)BarbersAndRebarbs$(EXE) $(RELEASEDIR)
	cp -r $(ASSETDIR) $(RELEASEDIR)
	cp --target-directory=$(RELEASEDIR) $(foreach lib,$(filter-out cpp-nbt,$(LDDLLS) libgcc_s_dw2-1 libstdc++-6), $(DLLDIR)$(lib)$(DLL)) $(CUSTOM_DLLS)
	$(STRIP) $(STRIPAR) $(RELEASEDIR)/*$(EXE) $(RELEASEDIR)/*$(DLL)
	7z a -r -y $(RELEASEDIR)/release.zip $(RELEASEDIR)/*

git :
	@pushd "ext/Cpp-NBT" 1>$(devnull) 2>$(devnull) && git pull -q -n && popd 1>$(devnull) 2>$(devnull)


$(OBJDIR)%$(OBJ) : $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPAR) -Iext/All -Iext/cereal/include -DCEREAL_VERSION='$(CEREAL_VERSION)' -c -o$@ $^
