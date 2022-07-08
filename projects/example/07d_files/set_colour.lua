function setColour(colour)
  local colours = {}
  colours["red"] = { "Red", Colours.WHITE, Colours.RED }
  colours["blue"] = { "Blue", Colours.WHITE, Colours.BLUE }
  colours["green"] = { "Green", Colours.WHITE, Colours.GREEN }
  colours["yellow"] = { "Yellow", Colours.BLACK, Colours.YELLOW }

  if colours[colour] then
    return colours[colour]
  else
    return { "???", Colours.BLACK, Colours.WHITE }
  end
end