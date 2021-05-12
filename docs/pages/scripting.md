LUA Scripting
=============

## Data Types

### vec2

### ivec2

### vec3

### Glyph

### Object

## Events

### Init() => `boolean`

### Loop(`float` _timestep_)

### Quit() => `boolean`

## Globals

### mouse.LEFT => `integer`

### mouse.MIDDLE => `integer`

### mouse.RIGHT => `integer`

### mouse.position => `ivec2`

### mouse.movement => `ivec2`

## Functions

### mouse.isDown(`integer` _button_) => `boolean`

### mouse.isPressed(`integer` _button_) => `boolean`

### mouse.isReleased(`integer` _button_) => `boolean`

### keyboard.isDown(`string` _key_) => `boolean`

### keyboard.isPressed(`string` _key_) => `boolean`

### keyboard.isReleased(`string` _key_) => `boolean`

### wireframe.enable()

### wireframe.disable()

### getFileList(`string` _directory_) => `Array<string>`

### getFolderList(`string` _directory_) => `Array<string>`

### addBox(`string` _name_, `vec2` _position_, `ivec2` _size_, `Glyph` _content_) => `Object`

### addOutlinedBox(`string` _name_, `vec2` _position_, `ivec2` _size_, `Glyph` _content_) => `Object`

### addText(`string` _name_, `vec2` _position_, `ivec2` _size_, `string` _text_, `vec3` _foreground_color_, `vec3` _background_color_) => `Object`