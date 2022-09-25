local timingText, timingValue

function Init()
  timingText = TextObject(Values.VEC2_ONE, ivec2(20, 2))
  timingText.setText("This text changes as time progresses!")

  timingValue = 0

  return true
end

function Loop(timestep)
  timingValue = timingValue + (timestep * 1.5)

  if timingValue >= 360 then timingValue = 0 end

  timingText.setColours(Colours.WHITE, vec3((math.sin(timingValue) * 127) + 128, 0.0, 0.0))
end

function Quit()
  timingText:release()
  
  return true
end