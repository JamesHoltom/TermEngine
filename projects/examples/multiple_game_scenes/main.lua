local otherGameScene, defaultText, otherText

function Init()
  otherGameScene = GameScene("my_game_scene")

  defaultText = TextObject(Values.IVEC2_ZERO, Ivec2(32, 2), "This is the default game scene!\nPress 't' to change scenes")
  defaultText.fg_colour = Colours.RED

  otherText = TextObject(Values.IVEC2_ZERO, Ivec2(32, 1), "This is my own game scene!", otherGameScene)
  otherText.fg_colour = Colours.BLUE
  otherText.bg_colour = Colours.WHITE

  return true
end

function Loop(timestep)
  if keyboard.isPressed("t") then
    if defaultWindow.gameScene == defaultGameScene then
      defaultWindow.gameScene = otherGameScene
    else
      defaultWindow.gameScene = defaultGameScene
    end
  end
end