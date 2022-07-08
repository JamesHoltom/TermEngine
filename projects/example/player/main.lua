-- External scripts can be loaded in locally.
require("player")

local player, outline, text, fpsText

--[[ 
	Init() is called once when the project is loaded.
	Return false to indicate that there is a problem at this stage.
]]--
function Init()
	player = Player(vec2(4))
	
	--[[
		BoxObjects 
	]]--
	outline = BoxObject(vec2(0, 0), ivec2(32, 16))
	outline.setOutline(Glyph("+", glyphs.DEFAULT_FOREGROUND_COLOUR, glyphs.DEFAULT_BACKGROUND_COLOUR))
	
	text = TextObject(vec2(1, 14), ivec2(30, 1))
	text.setText("Welcome to TermEngine!")

	fpsText = TextObject(vec2(24, 1), ivec2(7, 1))
	
	return true
end

--[[
	Loop() is called every frame, with the amount of time since the last frame.
]]--
function Loop(timestep)
	player.doInput(timestep)
	fpsText.setText(fps.average())
end

--[[
	Quit() is called when the user opts to exit the game.
	Return false to prevent the game from closing (e.g. to ask to save before exiting, etc.).
]]--
function Quit()
	return true
end
