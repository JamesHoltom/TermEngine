local timingText, timingValue, fpsText, debugText

function Init()
  fps.target = 60

  defaultScene.charmap.size = ivec2(32, 19)
  defaultScene:resizeToFit()

  timingText = TextObject(Values.IVEC2_ONE, ivec2(20, 2))
  timingText.text = "This text changes as time progresses!"

  timingValue = 0.0

  fpsText = TextObject(ivec2(24, 0), ivec2(8, 1))
  debugText = TextObject(ivec2(0, 3), ivec2(32, 16))

  return true
end

function Loop(timestep)
  timingValue = math.fmod(timingValue + (timestep * 1.5), 360.0)
  local fgValue = (math.sin(timingValue) * 127.5) + 127.5
  local bgValueR = 255.0 - ((math.sin(timingValue) * 127.5) + 127.5)
  local bgValueG = (math.cos(timingValue) * 127.5) + 127.5
  local bgValueB = (math.sin(timingValue) * 127.5) + 127.5

  timingText.bg_colour = vec4(bgValueR, bgValueG, bgValueB, 255.0)
  fpsText.text = fps.getAverage()
  debugText.text = getDebugInfo()
end
