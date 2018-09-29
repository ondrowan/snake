# Snake

A simple terminal snake game which served as a learning project for C++ and
its ecosystem. Therefore the code is most likely not ideal and full of various
rookie mistakes. If you find some, please report them so I can learn something
new.


## Building

[CMake](https://cmake.org/) and [ncurses](https://invisible-island.net/ncurses/)
are prerequisites.

* Clone this repository.
* Create build directory: `mkdir build`
* Change into build directory: `cd build`
* Create makefiles: `cmake -G 'Unix Makefiles' ..`
* If everything goes well, you should be able to run the game: `./snake`


## Note about optional.h

At the time being MacOS doesn't contain this header (only as
`experimental/optional` which is missing some methods) and I wanted to try it. I
had to add it directly to the repository.


## My learning goals

* Trying out C++ and its ecosystem:
  * Writing first real(-ish) program
  * CMake
  * clang
* Writing the simplest possible game
