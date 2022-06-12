local inputText, inputBox

function Init()
  inputText = TextObject(Values.VEC2_ONE, ivec2(12, 1))
  inputText.setText("Press W & S!")

  inputBox = BoxObject(Values.VEC2_ZERO, ivec2(14, 3))
  inputBox.setOutline(Glyph("#", Colours.WHITE, Colours.BLACK))

  return true
end

function Loop(timestep)
  if keyboard.isPressed("w") then
    print("Pressed W!")
  elseif keyboard.isReleased("w") then
    print("Released W!")
  end

  if keyboard.isDown("s") then
    inputBox.setOutline(Glyph("!", Colours.WHITE, Colours.RED))
  else
    inputBox.setOutline(Glyph("#", Colours.WHITE, Colours.BLACK))
  end
end
