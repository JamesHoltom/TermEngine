--[[
    This project uses fonts from Google Fonts, and are distributed under the Open Font License.

    - Space Mono, by Colophon: https://fonts.google.com/specimen/Space+Mono

    Open Font License: https://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&id=OFL
  ]]

local otherFont
local line1Text, line2Text, box

function Init()
  otherFont = Font("fonts/SpaceMono-Regular.ttf")

  print(defaultFont.filepath)
  print(otherFont.filepath)

  defaultScene.charmap.size = ivec2(40, 4)
  defaultScene:resizeToCharacterMap()

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
  if keyboard.isPressed("f") then
    if defaultScene.font == defaultFont then
      defaultScene.font = otherFont
    else
      defaultScene.font = defaultFont
    end

    print("Font Path: " .. defaultScene.font.filepath)
  elseif keyboard.isPressed("s") then
    if defaultScene.fontSize == 32 then
      defaultScene.fontSize = 48
    else
      defaultScene.fontSize = 32
    end

    print("Font Size: " .. defaultScene.fontSize)
  end
end