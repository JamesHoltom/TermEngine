local inputText, inputBox

function Init()
  inputText = TextObject(Values.VEC2_ONE, ivec2(12, 1))
  inputText.text = "Press W & S!"

  inputBox = BoxObject(Values.VEC2_ZERO, ivec2(14, 3))
  inputBox.outline = Glyph("#", Colours.WHITE, Colours.BLACK)

  return true
end

function Loop(timestep)
  if keyboard.isPressed("w") then
    print("Pressed W!")
  elseif keyboard.isReleased("w") then
    print("Released W!")
  end

  if keyboard.isDown("s") then
    inputBox.outline = Glyph("!", Colours.WHITE, Colours.RED)
  else
    inputBox.outline = Glyph("#", Colours.WHITE, Colours.BLACK)
  end
end

function Quit()
  inputText:release()
  inputBox:release()
  
  return true
end