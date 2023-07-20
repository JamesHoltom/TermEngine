local bgText, showBg, background1, background2

function Init()
  bgText = TextObject(Values.IVEC2_ZERO, Ivec2(19, 2))
  bgText.text = "Press 'b' to changethe background"

  showBg = 0
  background1 = "backgrounds/example.png"
  background2 = "backgrounds/danger.png"

  defaultScene.background.renderMode = true

  return true
end

function Loop(timestep)
  local bgChanged = false

  if keyboard.isPressed("b") then
    showBg = (showBg + 1) % 3

    if showBg == 0 then
      defaultScene.background.source = ""
    elseif showBg == 1 then
      defaultScene.background.source = background1
    else
      defaultScene.background.source = background2
    end
  elseif keyboard.isPressed("a") and defaultScene.background then
    if defaultScene.background.colour.a == 255 then
      defaultScene.background.colour.a = 192
    elseif defaultScene.background.colour.a == 192 then
      defaultScene.background.colour.a = 128
    elseif defaultScene.background.colour.a == 128 then
      defaultScene.background.colour.a = 64
    else
      defaultScene.background.colour.a = 255
    end
  end
end
