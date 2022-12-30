local fileText
local labelObject, valueObject

function SetColour(colour)
  if colour == "green" then
    return { "Green", Colours.WHITE, Colours.GREEN }
  elseif colour == "blue" then
    return { "Blue", Colours.WHITE, Colours.BLUE }
  elseif colour == "yellow" then
    return { "Yellow", Colours.BLACK, Colours.YELLOW }
  else
    return { "Red", Colours.WHITE, Colours.RED }
  end
end

function SetValue()
  local valueColours = SetColour(fileText)
  valueObject.text = "Value: " .. valueColours[1]
  valueObject.fg_colour = valueColours[2]
  valueObject.bg_colour = valueColours[3]
end

function Init()
  view.setSize(ivec2(16, 8))
  window.fitToView()

  if fs.exists("resources/myFile.txt") then
    fileText = fs.read("resources/myFile.txt")
  else
    fileText = "red"
  end

  labelObject = TextObject(Values.IVEC2_ONE, ivec2(14, 3))
  labelObject.text = "Press 't' to\nchange the\nvalue!"

  valueObject = TextObject(ivec2(1, 5), ivec2(14, 1))
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
  fs.write("resources/myFile.txt", fileText, false)

  return true
end
