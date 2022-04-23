local currentPath, currentSize
local fontText

function Init()
  usingDefault = true
  currentPath = font.defaultPath()
  currentSize = 0

  fontText = TextObject(Values.VEC2_ONE, ivec2(28, 2))
  fontText.setText("Press 'f' to change the fontPress 's' to change the size")

  return true
end

function Loop(timestep)
  local hasChanged = false

  if keyboard.isPressed("f") then
    if currentPath == font.defaultPath() then
      currentPath = "resources/fonts/SpaceMono-Regular.ttf"
    else
      currentPath = font.defaultPath()
    end

    hasChanged = true
  elseif keyboard.isPressed("s") then
    currentSize = (currentSize + 4) % 12
    hasChanged = true
  end

  if hasChanged then
    if not font.set(currentPath, font.defaultSize() + currentSize) then
      currentPath = font.defaultPath()
      print "Error!"
    end
  end
end