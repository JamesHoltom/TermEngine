--[[
    This project uses fonts from Google Fonts, and are distributed under the Open Font License.

    - Space Mono, by Colophon: https://fonts.google.com/specimen/Space+Mono

    Open Font License: https://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&id=OFL
  ]]

local currentPath, currentSize
local line1Text, line2Text, box

function Init()
  currentPath = font.getDefault()
  currentSize = font.getDefaultSize()

  defaultScene.charmap.size = ivec2(40, 4)
  defaultScene:resizeToFit()

  box = BoxObject(Values.IVEC2_ZERO, ivec2(40, 4))
  box.outline = Character("^", Colours.WHITE, Colours.RED)

  line1Text = TextObject(Values.IVEC2_ONE, ivec2(38, 1))
  line1Text.text = "Press 'f' to change the font"

  line2Text = TextObject(ivec2(1, 2), ivec2(38, 1))
  line2Text.text = "Press 's' to change the font size"
  line2Text.bg_colour = Colours.DARK_GREY

  return true
end

function Loop(timestep)
  local hasChanged = false

  if keyboard.isPressed("f") then
    if currentPath == font.getDefault() then
      currentPath = "fonts/SpaceMono-Regular.ttf"
    else
      currentPath = font.getDefault()
    end

    print("Font Path: " .. currentPath)

    hasChanged = true
  elseif keyboard.isPressed("s") then
    if currentSize == font.getDefaultSize() then
      currentSize = 48
    else
      currentSize = font.getDefaultSize()
    end

    print("Font Size: " .. currentSize)

    hasChanged = true
  end

  if hasChanged then
    local newFont = font.load(currentPath)

    defaultScene.font = newFont
    defaultScene.fontSize = currentSize
  end
end