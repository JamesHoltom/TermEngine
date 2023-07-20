-- External scripts can be loaded in locally.
require("player")

local player, outline, helloText

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
	player = Player(Ivec2(4))
	
	--[[
		BoxObjects 
	]]--
	outline = BoxObject(Ivec2(0, 0), Ivec2(32, 16))
	outline.outline = Character("+", characters.DEFAULT_FOREGROUND_COLOUR, characters.DEFAULT_BACKGROUND_COLOUR)
	
	helloText = TextObject(Ivec2(1, 14), Ivec2(30, 1))
	helloText.text = "Welcome to TermEngine!"

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
	player:update(timestep)
end
