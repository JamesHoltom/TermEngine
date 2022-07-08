local bgText, showBg

function Init()
  view.setSize(ivec2(19, 2))
  window.setSize(ivec2(640, 512))

  bgText = TextObject(Values.VEC2_ZERO, ivec2(19, 2))
  bgText.setText("Press 'b' to changethe background")

  showBg = 0

  return true
end

function Loop(timestep)
  if keyboard.isPressed("b") then
    showBg = (showBg + 1) % 3

    if showBg == 0 then
      background.remove()
    elseif showBg == 1 then
      background.set("backgrounds/example.png", vec2(64.0, 0.0), Colours.WHITE)
    else
      background.set("backgrounds/danger.png", vec2(64.0, 0.0), Colours.WHITE)
    end
  end
end
