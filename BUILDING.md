## Prerequisites

TermEngine requires the following libraries to build:

* SDL2
* OpenGL 4.4
* OpenGL Extension Wrangler (GLEW)
* OpenGL Mathematics (GLM)
* FreeType, which is dependent on:
	* Brotli
	* bzip2
	* libpng
	* zlib
* sol2
	* Lua 5.3
* spdlog, which is dependent on:
	* fmt
* cxxopts

The instructions below assume you have installed these libraries through vcpkg.

## Visual Studio 2019

Open `termengine.sln` in Visual Studio and build the project.

## VSCode

This project requires that the _CMake_ and _CMake Tools_ extensions for VSCode are installed.

Open the folder in VSCode, then configure and build the project.

## CMake

This project requires CMake version 3.16 and over.

Create the build directory by running `mkdir build`.

_Debug_:
- Create the makefiles: `cmake -DCMAKE_TOOLCHAIN_FILE:STRING=~/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE:STRING=Debug -S . -B build`
- Build the project: `cmake --build build --config Debug --target TermEngine`

_Release_:
- Create the makefiles: `cmake -DCMAKE_TOOLCHAIN_FILE:STRING=~/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE:STRING=Release -S . -B build`
- Build the project: `cmake --build build --config Release --target TermEngine`

If you want to set the C++ compiler used, add `-DCMAKE_CXX_COMPILER:FILEPATH=/path/to/compiler` when creating the makefiles.
E.g. to set g++ as the compiler, run `cmake -DCMAKE_TOOLCHAIN_FILE:STRING=~/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_CXX_COMPILER:FILEPATH=/bin/x86_64-linux-gnu-g++-9 -S . -B build`