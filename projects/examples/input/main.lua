local inputText, inputBox

function Init()
  inputText = TextObject(Values.IVEC2_ONE, Ivec2(12, 1))
  inputText.text = "Press W & S!"

  inputBox = BoxObject(Values.IVEC2_ZERO, Ivec2(14, 3))
  inputBox.outline = Character("#", Colours.WHITE, Colours.BLACK)

  return true
end

function Loop(timestep)
  if keyboard.isPressed("w") then
    print("Pressed W!")
  elseif keyboard.isReleased("w") then
    print("Released W!")
  end

  if keyboard.isDown("s") then
    inputBox.outline = Character("!", Colours.WHITE, Colours.RED)
  else
    inputBox.outline = Character("#", Colours.WHITE, Colours.BLACK)
  end
end
