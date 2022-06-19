local currentPath, currentSize, isSquare
local line1Text, line2Text, line3Text, box

function Init()
  currentPath = font.defaultPath()
  currentSize = 0
  isSquare = true

  view.setSize(ivec2(40, 5))
  window.fitToView()

  box = BoxObject(Values.VEC2_ZERO, view.getSize())
  box.setOutline(Glyph("^", Colours.WHITE, Colours.RED))

  line1Text = TextObject(Values.VEC2_ONE, ivec2(37, 1))
  line1Text.setText("Press 'f' to change the font      ")

  line2Text = TextObject(vec2(1, 2), ivec2(37, 1))
  line2Text.setText("Press 's' to change the font size ")
  line2Text.setColours(Colours.WHITE, Colours.DARK_GREY)

  line3Text = TextObject(vec2(1, 3), ivec2(37, 1))
  line3Text.setText("Press 'q' to change the glyph size")
  line3Text.setColours(Colours.WHITE, Colours.LIGHT_GREY)

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
  elseif keyboard.isPressed("q") then
    isSquare = not isSquare
    
    font.isSquare(isSquare)
    window.fitToView()
  end

  if hasChanged then
    if not font.set(currentPath, font.defaultSize() + currentSize, isSquare) then
      currentPath = font.defaultPath()
      print "Error!"
    else
      window.fitToView()
    end
  end
end