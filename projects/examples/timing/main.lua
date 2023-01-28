local timingText, timingValue

function Init()
  timingText = TextObject(defaultScene(), Values.IVEC2_ONE, ivec2(20, 2))
  timingText.text = "This text changes as time progresses!"

  timingValue = 0.0

  return true
end

function Loop(timestep)
  timingValue = math.fmod(timingValue + (timestep * 1.5), 360.0)
  local fgValue = (math.sin(timingValue) * 127.5) + 127.5
  local bgValueR = 255.0 - ((math.sin(timingValue) * 127.5) + 127.5)
  local bgValueG = (math.cos(timingValue) * 127.5) + 127.5
  local bgValueB = (math.sin(timingValue) * 127.5) + 127.5

  timingText.fg_colour = vec4(vec3(255.0), fgValue)
  timingText.bg_colour = vec4(bgValueR, bgValueG, bgValueB, 255.0) 
end
