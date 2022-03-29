TermEngine
==========

TermEngine is an engine that focuses on creating text-based games, with additional options for graphical features.

This project is currently a work-in-progress, with intermittent updates.

## Example

```
local hello_world

function Init()
  hello_world = TextObject(ivec2(1, 1), ivec2(12, 1))
	hello_world.setText("Hello World!")
  
  return true
end

```

Additional examples can be found in __projects/example__.

## Download

Clone a copy of the repository here:

`git clone https://github.com/JamesHoltom/TermEngine.git`

# Building

TermEngine can currently be been built on Windows and Linux.

Instructions can be found in __BUILDING__.

# Documentation

Source code documentation can be built with [Doxygen](https://www.doxygen.org/index.html).

To build the documentation, run `doxygen doxygen.conf`.

## License

TermEngine is released under the MIT license. License information can be found in __LICENSE__.

Licensing information for TermEngine's dependencies can be found in the __licenses__ directory.
