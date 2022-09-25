local aText, aBox

function Init()
  aText = TextObject(Values.VEC2_ONE, ivec2(12, 1))
	aText.text = "Hello World!"

  local aGlyph = Glyph('/', Colours.WHITE, Colours.BLACK)
  aBox = BoxObject(vec2(1, 3), ivec2(12, 4))
  aBox.fill = aGlyph

  return true
end

function Quit()
  aText:release()
  aBox:release()

  return true
end