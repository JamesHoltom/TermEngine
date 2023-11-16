local fileText
local labelObject, valueObject

function SetColour(colour)
  if colour == "green" then
    return "Green", Colours.WHITE, Colours.GREEN
  elseif colour == "blue" then
    return "Blue", Colours.WHITE, Colours.BLUE
  elseif colour == "yellow" then
    return "Yellow", Colours.BLACK, Colours.YELLOW
  else
    return "Red", Colours.WHITE, Colours.RED
  end
end

function SetValue()
  local colourText, fgColour, bgColour = SetColour(fileText)
  valueObject.text = "Value: " .. colourText
  valueObject.fg_colour = fgColour
  valueObject.bg_colour = bgColour
end

function Init()
  defaultGameScene.charmap.size = Ivec2(16, 8)
  defaultWindow:resizeToCharacterMap()

  if filesystem.exists("resources/myFile.txt") then
    fileText = filesystem.read("resources/myFile.txt")
  else
    fileText = "red"
  end

  labelObject = TextObject(Values.IVEC2_ONE, Ivec2(14, 3))
  labelObject.text = "Press 't' to\nchange the\nvalue!"

  valueObject = TextObject(Ivec2(1, 5), Ivec2(14, 1))
  SetValue()

  return true
end

function Loop(timestep)
  if keyboard.isPressed("t") then
    if fileText == "red" then
      fileText = "green"
    elseif fileText == "green" then
      fileText = "blue"
    elseif fileText == "blue" then
      fileText = "yellow"
    else
      fileText = "red"
    end

    SetValue()
  end
end

function Quit()
  filesystem.write("resources/myFile.txt", fileText, false)
end
