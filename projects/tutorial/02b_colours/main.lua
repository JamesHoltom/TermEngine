local colourText

function Init()
  local i = 0

  for k, v in pairs(Colours) do
    i = i + 1

    -- Change the text colour if the background is too bright.
    local isBright = (v == Colours.YELLOW or v == Colours.WHITE or v == Colours.LIGHT_GREY or v == Colours.CYAN)
    colourText = TextObject(vec2(1, i), ivec2(15, 1))
    colourText.text = "Colourful text!"
    colourText.bg_colour = v

    if isBright then
      colourText.fg_colour = Colours.BLACK
    else
      colourText.fg_colour = Colours.WHITE
    end
  end

  return true
end

function Quit()
  colourText:release()

  return true
end