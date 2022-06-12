local bgText, showBg

function Init()
  view.setSize(ivec2(19, 2))
  window.setSize(ivec2(640, 512))

  bgText = TextObject(Values.VEC2_ZERO, ivec2(19, 2))
  bgText.setText("Press 'b' to changethe background")

  showBg = false

  return true
end

function Loop(timestep)
  if keyboard.isPressed("b") then
    showBg =  not showBg

    if showBg then
      background.set("example.png", vec2(64.0, 0.0), Colours.WHITE)
    else
      background.remove()
    end
  end
end
