function Init()
  local aText = TextObject(Values.VEC2_ONE, ivec2(12, 1))
	aText.setText("Hello World!")

  local aGlyph = Glyph('/', Colours.WHITE, Colours.BLACK)
  local aBox = BoxObject(vec2(1, 3), ivec2(12, 4))
  aBox.setFill(aGlyph)

  return true
end