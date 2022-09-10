--[[
  || The Init() function runs once, when the project is loaded.
  ||
  || This is where you would run code to load assets, do setup, etc.
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
  || The Quit() function can run multiple times, whenever the user tries to exit TermEngine.
  ||
  || This is where you would run code to tear down assets, save game data, etc.
  ||
  || Like Init(), this function must return a true/false value.
  || If you want the user to confirm they want to exit (e.g. A "Would you like to save?" dialog),
  || you can pass false to prevent TermEngine from automatically closing.
--]]
function Quit()
  print("Goodbye World!")
  
  return true
end