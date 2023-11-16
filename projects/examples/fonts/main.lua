--[[
    This project uses fonts from Google Fonts, and are distributed under the Open Font License.

    - Space Mono, by Colophon: https://fonts.google.com/specimen/Space+Mono

    Open Font License: https://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&id=OFL
  ]]

local otherFont
local line1Text, line2Text, box

function Init()
  otherFont = Font("fonts/SpaceMono-Regular.ttf")

  defaultGameScene.charmap.size = Ivec2(40, 4)
  defaultWindow:resizeToCharacterMap()

  box = BoxObject(Values.IVEC2_ZERO, Ivec2(40, 4))
  box.outline = Character("^", Colours.WHITE, Colours.RED)

  line1Text = TextObject(Values.IVEC2_ONE, Ivec2(38, 1))
  line1Text.text = "Press 'f' to change the font"

  line2Text = TextObject(Ivec2(1, 2), Ivec2(38, 1))
  line2Text.text = "Press 's' to change the font size"
  line2Text.bg_colour = Colours.DARK_GREY

  return true
end

function Loop(timestep)
  if keyboard.isPressed("f") then
    if defaultWindow.font == defaultFont then
      defaultWindow.font = otherFont
    else
      defaultWindow.font = defaultFont
    end

    print("Font Path: " .. defaultWindow.font.name)
  elseif keyboard.isPressed("s") then
    if defaultWindow.fontSize == 32 then
      defaultWindow.fontSize = 48
    else
      defaultWindow.fontSize = 32
    end

    print("Font Size: " .. defaultWindow.fontSize)
  end
end