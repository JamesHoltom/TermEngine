local timingText, timingValue, fpsText

function Init()
  timingText = TextObject(Values.IVEC2_ONE, ivec2(20, 2))
  timingText.text = "This text changes as time progresses!"

  timingValue = 0.0

  fpsText = TextObject(ivec2(24, 0), ivec2(8, 1))

  return true
end

function Loop(timestep)
  timingValue = math.fmod(timingValue + (timestep * 0.003), 360.0)
  local fgValue = (math.sin(timingValue) * 127.5) + 127.5
  local bgValueR = 255.0 - ((math.sin(timingValue) * 127.5) + 127.5)
  local bgValueG = (math.cos(timingValue) * 127.5) + 127.5
  local bgValueB = (math.sin(timingValue) * 127.5) + 127.5

  timingText.bg_colour = vec4(bgValueR, bgValueG, bgValueB, 255.0)
  fpsText.text = string.sub(fps.getAverage(), 0, 10)
end
