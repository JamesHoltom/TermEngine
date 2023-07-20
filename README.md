# TermEngine

TermEngine is a game engine that focuses on creating text-based games.

## Example

```lua
local hello_world

function Init()
  hello_world = TextObject("Hello World!", Values.IVEC2_ONE, Ivec2(12, 1))
  
  return true
end

```

Additional examples can be found in __projects/example__.

## Building

TermEngine can currently be been built on Windows, Mac and Linux.

Instructions can be found in __BUILDING__.

## Documentation

Source code documentation can be built with [Doxygen](https://www.doxygen.org/index.html).

To build the documentation, run `doxygen doxygen.conf`.

Documentation for the Lua API can be found at the [wiki](https://github.com/JamesHoltom/TermEngine/wiki).

## License

TermEngine is released under the MIT license. License information can be found in __LICENSE__.

Licensing information for TermEngine's dependencies can be found in the __licenses__ directory.
