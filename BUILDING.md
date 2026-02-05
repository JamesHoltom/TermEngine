# Building TermEngine

## Download

You can clone TermEngine by running:

`git clone https://github.com/JamesHoltom/TermEngine.git`

## Prerequisites

This project is configured to build with the following libraries:

* cxxopts 3.0.0
* FreeType 2.12.1
* OpenGL 4.6
* OpenGL Mathematics (GLM) 0.9.9
* OpenGL Extensions Wrangler (GLEW) 2.2.0
* Lua 5.3
* SDL2 2.32.10
* sol2 3.3.0

cxxopts, GLM, SDL2 and sol2 are provided as Git submodules, and the others are assumed to be installed on your system.

## CMake

This project requires CMake version 3.16 and over.

Create the build directory by running `mkdir build`.

_Debug_:

* Create the makefiles: `cmake -DCMAKE_BUILD_TYPE:STRING=Debug -S . -B build`
* Build the project: `cmake --build build --config Debug --target TermEngine`

_Release_:

* Create the makefiles: `cmake -DCMAKE_BUILD_TYPE:STRING=Release -S . -B build`
* Build the project: `cmake --build build --config Release --target TermEngine`

If you want to set the C++ compiler used, add `-DCMAKE_CXX_COMPILER:FILEPATH=/path/to/compiler` when creating the makefiles.
E.g. to set g++ as the compiler, run `cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_CXX_COMPILER:FILEPATH=/bin/x86_64-linux-gnu-g++-9 -S . -B build`
