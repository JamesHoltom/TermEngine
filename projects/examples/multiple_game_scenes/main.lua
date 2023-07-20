local otherGameScene, defaultText, otherText

function Init()
  otherGameScene = GameScene("my_game_scene")

  defaultText = TextObject(Values.IVEC2_ZERO, Ivec2(20, 2), "This is the default\ngame scene!")
  defaultText.fg_colour = Colours.RED

  otherText = TextObject(Values.IVEC2_ZERO, Ivec2(20, 2), "This is my own\ngame scene!", "my_game_scene")
  otherText.fg_colour = Colours.BLUE

  return true
end