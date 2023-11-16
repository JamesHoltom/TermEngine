local bgText, showBg, background1, background2

function Init()
  bgText = TextObject(Values.IVEC2_ZERO, Ivec2(20, 2), "Press 'b' to change\nthe background")

  showBg = 0
  background1 = "backgrounds/example.png"
  background2 = "backgrounds/danger.png"

  return true
end

function Loop(timestep)
  local bgChanged = false

  if keyboard.isPressed("b") then
    showBg = (showBg + 1) % 3

    if showBg == 0 then
      defaultWindow.background:remove()
    elseif showBg == 1 then
      defaultWindow.background.source = background1
    else
      defaultWindow.background.source = background2
    end
  elseif keyboard.isPressed("a") and defaultWindow.background then
    if defaultWindow.background.colour.a == 255 then
      defaultWindow.background.colour.a = 192
    elseif defaultWindow.background.colour.a == 192 then
      defaultWindow.background.colour.a = 128
    elseif defaultWindow.background.colour.a == 128 then
      defaultWindow.background.colour.a = 64
    else
      defaultWindow.background.colour.a = 255
    end
  end
end
