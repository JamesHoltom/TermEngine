local timingText, timingValue

function Init()
  timingText = TextObject(Values.IVEC2_ONE, ivec2(20, 2))
  timingText.text = "This text changes as time progresses!"
  timingText.fg_colour = Colours.WHITE

  timingValue = 0

  return true
end

function Loop(timestep)
  timingValue = timingValue + (timestep * 1.5)

  if timingValue >= 360 then timingValue = 0 end

  timingText.bg_colour = vec4((math.sin(timingValue) * 127) + 128, 0.0, 0.0, 255.0)
end
