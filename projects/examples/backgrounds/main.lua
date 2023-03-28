local scene, bgText, showBg, bg, alpha

function Init()
  bgText = TextObject(Values.IVEC2_ZERO, ivec2(19, 2))
  bgText.text = "Press 'b' to changethe background"

  showBg = 0
  bg = ""
  alpha = 255

  return true
end

function Loop(timestep)
  local bgChanged = false

  if keyboard.isPressed("b") then
    showBg = (showBg + 1) % 3

    if showBg == 0 then
      bg = ""
    elseif showBg == 1 then
      bg = "backgrounds/example.png"
    else
      bg = "backgrounds/danger.png"
    end

    bgChanged = true
  elseif keyboard.isPressed("a") then
    if alpha == 255 then
      alpha = 192
    elseif alpha == 192 then
      alpha = 128
    elseif alpha == 128 then
      alpha = 64
    else
      alpha = 255
    end

    bgChanged = true
  end

  if bgChanged then
    if bg == "" then
      defaultScene.background:unload()
    else
      defaultScene.background:load(bg)
    end
  end
end
