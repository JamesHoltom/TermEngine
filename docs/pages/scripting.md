LUA Scripting
=============

## Data Types

### vec2
A 2-dimensional vector, that takes floating-point numbers.

#### Constructors
- *vec2()*
- *vec2( xy `number` )*
- *vec2( x `number`, y `number` )*
- *vec2( vector `vec2` )*
- *vec2( vector `ivec2` )*

#### Parameters
- *x* `number`
- *y* `number`

### ivec2
A 2-dimensional vector, that takes integer numbers.

#### Constructors
- *ivec2()*
- *ivec2( xy `number` )*
- *ivec2( x `number`, y `number` )*
- *ivec2( vector `vec2` )*
- *ivec2( vector `ivec2` )*

#### Parameters
- *x* `number`
- *y* `number`

### vec3
A 3-dimensional vector, that takes floating-point numbers.

#### Constructors
- *vec3()*
- *vec3( xyz `number` )*
- *vec3( x `number`, y `number`, z `number` )*
- *vec3( xy `vec2`, z `number` )*
- *vec3( vector `vec3` )*

#### Parameters
- *x* `number`
- *y* `number`
- *z* `number`
- *r* `number`
- *g* `number`
- *b* `number`

### Glyph
Represents a character on the window, combined with its foreground/background colors.

#### Constructors
- *Glyph( character `string`, foreground_color `vec3`, background_color `vec3` )*

#### Parameters
- *character* `string`
- *foreground_color* `vec3`
- *background_color* `vec3`

### Object
Represents an object, or a collection of *Glyphs*.

#### Constructors
- *Object( position `vec2`, size `ivec2` )*

#### Parameters
- *position* `vec2`
- *size* `ivec2`

## Events

### Init

*Init()* `boolean` 

Fires when TermEngine has initialised.

If *false* is returned here, the application will close.

### Loop
Fires every frame.

#### Parameters
- *timestep* `number`

### Quit
Fires when the user closes TermEngine.

If false is returned here, the application will not close.

#### Returns
`boolean`

### OnKeyDown
Fires when a key is held down.

#### Parameters
- *key* `string`

### OnKeyPress
Fires when a key has been pressed.

#### Parameters
- *key* `string`

### OnKeyRelease
Fires when a key has been released.

#### Parameters
- *key* `string`

### OnMouseDown
Fires when a mouse button is held down.

#### Parameters
- *button* `number`

### OnMousePress
Fires when a mouse button has been pressed.

#### Parameters
- *button* `number`

### OnMouseRelease
Fires when a mouse button has been released.

#### Parameters
- *button* `number`

### OnMouseMove
Fires when the mouse moves.

## Namespaces

### background

- *set( filename `string` )*
- *remove()*

### file

- *getFileList( directory `string` )* `array`
- *getFolderList( directory `string` )* `array`

### glyphs

- *size* `vec2` Returns the size of glyphs, in pixels.
- *NO_CHARACTER* `string`
- *DEFAULT_FG* `vec3`
- *DEFAULT_BG* `vec3`

### keyboard

- *isDown( key `string` )* `boolean` Checks if a key is being held down.
- *isPressed( key `string` )* `boolean` Checks if a key has been pressed.
- *isReleased( key `string` )* `boolean` Checks if a key has been released.

### mouse

- *LEFT* `number`
- *MIDDLE* `number`
- *RIGHT* `number`
- *position* `ivec2`
- *movement* `ivec2`

- *isDown( button `number` )* `boolean` Checks if a button is being held down.
- *isPressed( button `number` )* `boolean` Checks if a button has been pressed.
- *isReleased( button `number` )* `boolean` Checks if a button has been released.

### objects

- *count()* `number` Returns the number of created objects.
- *dirty()* `void` Forces an update of all objects, redrawing them to the window.
- *is_dirty()* `boolean` 

### random

- *get()* `number` Returns a random number between *0* and *1*.
- *get( range `number` )* `number` Returns a random number between *0* and `range`.
- *get( range `number`, offset `number` )* `number` Returns a random number between `range` and `range + offset`.

### fps

- *print()* `void` Prints framerate information to the log.
- *average()* `number` Returns the average framerate of the program.
- *usingTarget()* `boolean` Returns if a target framerate is set.
- *getTarget()* `number` Returns the target framerate, or *0* if disabled.
- *setTarget( target `number` )* `void` Sets the target framerate. Setting it to *0* will disable the target framerate.
- *frames()* `number` Returns the number of frames since the program started.

### wireframe

- *enable()* `void` 
- *disable()* `void` 
