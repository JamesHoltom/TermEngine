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

*Loop( timestep `number` )* `void`

Fires every frame.

### Quit

*Quit()* `boolean`

Fires when the user closes TermEngine.

If *false* is returned here, the application will not close.

### OnKeyDown

*OnKeyDown( key `string` )* `void`

Fires when a *key* is held down.

### OnKeyPress

*OnKeyPress( key `string` )* `void`

Fires when a *key* has been pressed.

### OnKeyRelease

*OnKeyRelease( key `string` )* `void`

Fires when a *key* has been released.

### OnMouseDown

*OnMouseDown( button `number` )* `void`

Fires when a mouse *button* is held down.

### OnMousePress

*OnMousePress( button `number` )* `void`

Fires when a mouse *button* has been pressed.

### OnMouseRelease

*OnMouseRelease( button `number` )* `void`

Fires when a mouse *button* has been released.

### OnMouseMove

*OnMouseMove( delta `ivec2` )* `void`

Fires when the mouse moves.

## Namespaces

### background

- *background.set( filename `string` )*
- *background.remove()*

### file

- *file.getFileList( directory `string` )* `array`
- *file.getFolderList( directory `string` )* `array`

### glyphs

- *glyphs.size* `vec2` Returns the size of glyphs, in pixels.
- *glyphs.NO_CHARACTER* `string`
- *glyphs.DEFAULT_FG* `vec3`
- *glyphs.DEFAULT_BG* `vec3`

### keyboard

- *keyboard.isDown( key `string` )* `boolean` Checks if a key is being held down.
- *keyboard.isPressed( key `string` )* `boolean` Checks if a key has been pressed.
- *keyboard.isReleased( key `string` )* `boolean` Checks if a key has been released.

### mouse

- *mouse.LEFT* `number`
- *mouse.MIDDLE* `number`
- *mouse.RIGHT* `number`
- *mouse.position* `ivec2` The position of the mouse, in pixels (px).
- *mouse.movement* `ivec2` The amount of mouse movement since the last frame, in pixels (px).
- *mouse.isDown( button `number` )* `boolean` Checks if a button is being held down.
- *mouse.isPressed( button `number` )* `boolean` Checks if a button has been pressed.
- *mouse.isReleased( button `number` )* `boolean` Checks if a button has been released.

### objects

- *objects.count()* `number` Returns the number of created objects.
- *objects.dirty()* `void` Forces an update of all objects, redrawing them to the window.
- *objects.is_dirty()* `boolean` 

### random

- *random.get()* `number` Returns a random number between *0* and *1*.
- *random.get( range `number` )* `number` Returns a random number between *0* and `range`.
- *random.get( range `number`, offset `number` )* `number` Returns a random number between `range` and `range + offset`.

### fps

- *fps.print()* `void` Prints framerate information to the log.
- *fps.average()* `number` Returns the average framerate of the program.
- *fps.usingTarget()* `boolean` Returns if a target framerate is set.
- *fps.getTarget()* `number` Returns the target framerate, or *0* if disabled.
- *fps.setTarget( target `number` )* `void` Sets the target framerate. Setting it to *0* will disable the target framerate.
- *fps.frames()* `number` Returns the number of frames since the program started.

### wireframe

- *wireframe.enable()* `void` Enables wireframe mode. For debugging purposes.
- *wireframe.disable()* `void` Disables wireframe mode.
