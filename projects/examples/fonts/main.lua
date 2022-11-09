--[[
    This project uses fonts from Google Fonts, and are distributed under the Open Font License.

    - Space Mono, by Colophon: https://fonts.google.com/specimen/Space+Mono

    Open Font License: https://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&id=OFL
  ]]

local currentPath, currentSize, isSquare, characterHeight
local line1Text, line2Text, line3Text, box

function Init()
  currentPath = font.getDefaultPath()
  currentSize = 0
  isSquare = false
  characterHeight = font.getCharacterSize().y

  view.setSize(ivec2(40, 5))
  window.fitToView()

  box = BoxObject(Values.IVEC2_ZERO, view.getSize())
  box.outline = Character("^", Colours.WHITE, Colours.RED)

  line1Text = TextObject(Values.IVEC2_ONE, ivec2(37, 1))
  line1Text.text = "Press 'f' to change the font"

  line2Text = TextObject(ivec2(1, 2), ivec2(37, 1))
  line2Text.text = "Press 's' to change the font size"
  line2Text.bg_colour = Colours.DARK_GREY

  line3Text = TextObject(ivec2(1, 3), ivec2(37, 1))
  line3Text.text = "Press 'q' to change the character size"
  line3Text.bg_colour = Colours.LIGHT_GREY

  return true
end

function Loop(timestep)
  local hasChanged = false

  if keyboard.isPressed("f") then
    if currentPath == font.getDefaultPath() then
      currentPath = "fonts/SpaceMono-Regular.ttf"
    else
      currentPath = font.getDefaultPath()
    end

    print("Path: " .. currentPath)

    hasChanged = true
  elseif keyboard.isPressed("s") then
    currentSize = (currentSize + 4) % 12
    hasChanged = true
  elseif keyboard.isPressed("q") then
    isSquare = not isSquare
    
    if isSquare then
      font.resetCharacterSize()
    else
      font.setCharacterSize(ivec2(characterHeight))
    end

    window.fitToView()
  end

  if hasChanged then
    if not font.set(currentPath, font.getDefaultSize() + currentSize) then
      currentPath = font.getDefaultPath()
      print "Error!"
    else
      window.fitToView()
    end
  end
end