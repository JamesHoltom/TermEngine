require("set_colour")

local fileText
local labelObject, valueObject

function SetValue()
  local valueColours = setColour(fileText)
  valueObject.setText("Value: " .. valueColours[1])
  valueObject.setColours(valueColours[2], valueColours[3])
end

function Init()
  view.setSize(ivec2(16, 8))
  window.fitToView()

  if fs.exists("resources/myFile.txt") then
    fileText = fs.read("resources/myFile.txt")
  else
    fileText = "red"
  end

  labelObject = TextObject(Values.VEC2_ONE, ivec2(14, 3))
  labelObject.setText("Press 't' to\nchange the\nvalue!")

  valueObject = TextObject(vec2(1, 5), ivec2(14, 1))
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