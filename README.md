# TermEngine

TermEngine is an engine that focuses on creating text-based games, with additional options for graphical features.

This project is currently a work-in-progress, with intermittent updates.

## Building

TermEngine can currently be been built on Windows, with Linux support in the works.

### Prerequisites

TermEngine uses the following libraries:

* SDL2
* SDL2_image, which is dependent on:
	* libpng
	* zlib
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

Licensing information can be found in the __licenses__ directory.

### Download

Clone a copy of the repository here:

`git clone https://github.com/JamesHoltom/TermEngine.git`

### Visual Studio 2019

Open `termengine.sln` in Visual Studio and build the project.

### Makefile

Makefile support is being developed.

## Documentation

Source code documentation can be built with Doxygen.

To build the documentation, run `doxygen doxygen.conf`.
