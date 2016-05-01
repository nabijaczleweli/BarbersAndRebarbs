BarbersAndRebarbs [![Build Status](https://img.shields.io/travis/nabijaczleweli/BarbersAndRebarbs.svg?style=plastic)](https://travis-ci.org/nabijaczleweli/BarbersAndRebarbs) [![Licence](https://img.shields.io/badge/license-MIT-blue.svg?style=plastic)](LICENSE)
=======
A game in C++ using SFML 2.3

## Requirements
1. Audiere, found [here](http://audiere.sourceforge.net), is required to be installed externally
2. SFML, found [here](http://www.sfml-dev.org), is required to be installed externally
3. All other dependencies are submodules, and as suck myst be explicitly initialised, be it by passing `--recursive` to `git-clone` or running `git submodule update --init --recursive`

"Externally" installed libraries need to be installed either globally or to pass `ADDITIONAL_{INCLUDE,LINK}_DIR` variable to GNU Make, as appropriate.

For build/installation of Audiere refer to the [TravisCI Configuration file](.travis.yml), as it is quite the ordeal.
