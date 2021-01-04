# TermEngine

TermEngine is an engine that focuses on creating text-based games, with additional options for graphical features.

This project is currently a work-in-progress, with intermittent updates.

## Building

TermEngine can currently be been built on Windows, with Linux support in the works.

### Prerequisites

TermEngine requires the following libraries to build:

* SDL2 2.0.12
* OpenGL 4.4
* OpenGL Extension Wrangler (GLEW) 2.1.0
* OpenGL Mathematics (GLM) 9.9.7
* FreeType 2.10
* Lua 5.3
* sol3 3.2.0
* spdlog 1.6.0

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
