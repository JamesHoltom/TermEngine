function Init()
  view.setSize(ivec2(17, 14))
  window.fitToView()

  local i = 0

  for k, v in pairs(Colours) do
    i = i + 1

    -- Change the text colour if the background is too bright.
    local isBright = (v == Colours.YELLOW or v == Colours.WHITE)
    local colourText = TextObject(vec2(1, i), ivec2(15, 1))
    colourText.setText("Colourful text!")

    if isBright then
      colourText.setColours(Colours.BLACK, v)
    else
      colourText.setColours(Colours.WHITE, v)
    end
  end

  return true
end