--[[
  || The Init() function runs when a project is loaded.
  ||
  || This is where you would load assets, do setup, etc.
  ||
  || This function must return a true/false value.
  || If a problem occurs, or if you want to stop the project from running, you can pass false to
  || stop TermEngine.
--]]
function Init()
  print("Hello World!")

  return true
end

--[[
  || The Loop() function runs every frame.
  ||
  || This is where game logic is executed.
  ||
  || The function takes a timestep, with the length of time since the last frame.
--]]
function Loop(timestep)

end

--[[
  || The Quit() function runs whenever the user tries to exit TermEngine.
  ||
  || This is where you would tear down assets, save game data, etc.
--]]
function Quit()
  print("Goodbye World!")
end