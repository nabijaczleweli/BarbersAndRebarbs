BarbersAndRebarbs [![Build Status](https://img.shields.io/travis/nabijaczleweli/BarbersAndRebarbs.svg)](https://travis-ci.org/nabijaczleweli/BarbersAndRebarbs) [![Licence](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)
=======
A game in C++ using SFML 2.3

## Requirements
#### Libraries
1. Speex, found [here](http://www.speex.org/), via `libspeex-dev` APT package or `mingw-w64-x86_64-speex` MSYS2 `pacman` package is required to be installed externally
2. DUMB, found [here](http://dumb.sourceforge.net/) via `libdumb1-dev` APT package or `mingw-w64-x86_64-dumb` MSYS2 `pacman` package, is required to be installed externally
2. SFML >=2.3, found [here](http://sfml-dev.org/) via `libsfml-dev` APT package or `mingw-w64-x86_64-sfml` MSYS2 `pacman` package, is required to be installed externally
3. All other dependencies are submodules, and as such must be explicitly initialised, be it by passing `--recursive` to `git-clone` or running `git submodule update --init --recursive`

"Externally" installed libraries need to be installed either globally or to pass `ADDITIONAL_{INCLUDE,LINK}_DIR` variable to GNU Make, as appropriate.

#### Tools
1. Non-archaic `ninja`, found [here](https://ninja-build.org/) via `ninja-build` APT package or `mingw-w64-x86_64-ninja` MSYS2 `pacman` package, must be callable
2. >=3.0 CMake, found [here](http://cmake.org/) or via `cmake` package, must be callable

"Callable" tools must be either in `PATH` or passed in env as their uppercase names (`CMAKE=$HOME/bin/cmake-3.6.9`)
