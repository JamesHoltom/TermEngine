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
Fires when TermEngine has initialised.

If false is returned here, the application will close.

#### Returns
`boolean`

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

#### Functions
- *set( filename `string` )*
- *remove()*

### file

#### Functions
- *getFileList( directory `string` )* `array`
- *getFolderList( directory `string` )* `array`

### glyphs

#### Members
- *size* `vec2`
- *NO_CHARACTER* `string`
- *DEFAULT_FG* `vec3`
- *DEFAULT_BG* `vec3`

### keyboard

#### Functions
- *isDown( key `string` )* `boolean`
- *isPressed( key `string` )* `boolean`
- *isReleased( key `string` )* `boolean`

### mouse

#### Members
- *LEFT* `number`
- *MIDDLE* `number`
- *RIGHT* `number`
- *position* `ivec2`
- *movement* `ivec2`

#### Functions
- *isDown( button `number` )* `boolean`
- *isPressed( button `number` )* `boolean`
- *isReleased( button `number` )* `boolean`

### objects

#### Functions
- *count()* `number`
- *dirty()*
- *is_dirty()* `boolean`

### rand

#### Functions
- *get()* `number`
- *get( range `number` )* `number`
- *get( range `number`, offset `number` )* `number`

### wireframe

#### Functions
- *enable()*
- *disable()*
