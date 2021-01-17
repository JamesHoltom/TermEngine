* Utilise Uniform Buffer Objects (UBO's) to store common uniforms across shaders, e.g. the projection matrix.
* Replace the usage of `std::shared_ptr` with `std::unique_ptr`, if possible.
* Replace the `Scene` and `GlyphSet` Lua usertypes with functions. There is no need to have objects for this.
* Split the key of `FontAtlasList` to allow for the same font to have multiple sizes.
* Add `LoadProject` function to `ScriptInterface`.
* Add functions to set multiple glyphs at once (e.g. SetLine, SetBox, SetBorder, etc.).
* Set up Background class.
* Set up Audio class.