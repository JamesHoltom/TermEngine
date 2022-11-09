-- External scripts can be loaded in locally.
require("player")

local player, outline, helloText, fpsText

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
	player = Player(ivec2(4))
	
	--[[
		BoxObjects 
	]]--
	outline = BoxObject(ivec2(0, 0), ivec2(32, 16))
	outline.outline = Character("+", characters.DEFAULT_FOREGROUND_COLOUR, characters.DEFAULT_BACKGROUND_COLOUR)
	
	helloText = TextObject(ivec2(1, 14), ivec2(30, 1))
	helloText.text = "Welcome to TermEngine!"

	fpsText = TextObject(ivec2(24, 1), ivec2(7, 1))
	
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
	player.doInput(timestep)
	fpsText.text = fps.getAverage()
end

--[[
  || The Quit() function runs whenever the user tries to exit TermEngine.
  ||
  || This is where you would tear down assets, save game data, etc.
  ||
  || Like Init(), this function must return a true/false value.
  || If you want the user to confirm they want to exit (e.g. A "Would you like to save?" dialog),
  || you can pass false to prevent TermEngine from automatically closing.
--]]
function Quit()
	return true
end
