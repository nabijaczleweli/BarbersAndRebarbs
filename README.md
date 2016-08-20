BarbersAndRebarbs [![Build Status](https://img.shields.io/travis/nabijaczleweli/BarbersAndRebarbs.svg)](https://travis-ci.org/nabijaczleweli/BarbersAndRebarbs) [![Licence](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)
=======
A game in C++ using SFML 2.4

## Requirements
#### Libraries
1. Speex, found [here](http://www.speex.org/), via `libspeex-dev` APT package or `mingw-w64-x86_64-speex` MSYS2 `pacman` package is required to be installed externally
2. DUMB, found [here](http://dumb.sourceforge.net/) via `libdumb1-dev` APT package or `mingw-w64-x86_64-dumb` MSYS2 `pacman` package, is required to be installed externally
2. SFML >=2.4, found [here](http://sfml-dev.org/) via `libsfml-dev` APT package or `mingw-w64-x86_64-sfml` MSYS2 `pacman` package, is required to be installed externally
3. `libcurl`, found [here](https://curl.haxx.se/) via either of `libcurl4-{gnutls,nss,openssl}-dev` APT packages or `libcurl` MSYS2 `pacman` package, is required to be installed externally
4. All other dependencies are submodules, and as such must be explicitly initialised, be it by passing `--recursive` to `git-clone` or running `git submodule update --init --recursive`

"Externally" installed libraries need to be installed either globally or to pass `ADDITIONAL_{INCLUDE,LINK}_DIR` variable to GNU Make, as appropriate.

#### Tools
1. Non-archaic `ninja`, found [here](https://ninja-build.org/) via `ninja-build` APT package or `mingw-w64-x86_64-ninja` MSYS2 `pacman` package, must be callable
2. >=3.0 CMake, found [here](http://cmake.org/) or via `cmake` package, must be callable

"Callable" tools must be either in `PATH` or passed in env as their uppercase names (`CMAKE=$HOME/bin/cmake-3.6.9`)

## Library applications

1. [SFML](http://sfml-dev.org/) — general graphics and window display
2. [audiere](https://sourceforge.net/projects/audiere/) ([modified](https://github.com/nabijaczleweli/audiere)) — sounds
3. [cimpoler-meta](https://github.com/nabijaczleweli/cimpoler-meta) — purely informational compiler and system data
4. [cpp-localiser](https://github.com/nabijaczleweli/cpp-localiser) — localisation
4. [cpr](https://github.com/whoshuu/cpr) — GitHub API interaction
4. [fmt](https://github.com/fmtlib/fmt) — formatting in localisation
5. [jsonpp](https://github.com/Rapptz/jsonpp) — game saving/loading, model (`drawing`) loading
6. [seed11](https://github.com/milleniumbug/seed11) — true RNG seeding
7. [semver](https://github.com/zmarko/semver) — semantic versioning comparison
8. [whereami-cpp](https://github.com/nabijaczleweli/whereami-cpp) — asset directory localisation
9. [zstd](https://github.com/Cyan4973/zstd) — savefile compression
