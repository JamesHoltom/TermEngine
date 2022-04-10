local inputText, downBox

function Init()
  inputText = TextObject(Values.VEC2_ONE, ivec2(10, 1))
  inputText.setText("Press W!  ")

  downBox = BoxObject(Values.VEC2_ZERO, ivec2(12, 3))
  downBox.setOutline(Glyph("#", Colours.WHITE, Colours.BLACK))

  return true
end

function Loop(timestep)
  if keyboard.isPressed("w") then
    inputText.setColours(nil, Colours.RED)
  elseif keyboard.isReleased("w") then
    inputText.setColours(nil, Colours.ORANGE)
  else
    inputText.setColours(nil, Colours.BLACK)
  end

  if keyboard.isDown("s") then
    downBox.setOutline(Glyph("!", Colours.WHITE, Colours.RED))
  else
    downBox.setOutline(Glyph("#", Colours.WHITE, Colours.BLACK))
  end
end
