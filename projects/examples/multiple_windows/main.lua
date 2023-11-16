local otherGameWindow, objA, objB

function Init()
  otherGameWindow = GameWindow()

  objA = TextObject(Values.IVEC2_ZERO, Ivec2(20, 1), "This is window 1!", defaultGameScene)
  objB = TextObject(Values.IVEC2_ZERO, Ivec2(20, 1), "This is window 2!", otherGameWindow.gameScene)

  return true
end