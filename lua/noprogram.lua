--[[
-- @author James Holtom
--]]

local outline, noProgramText

function Init()
	outline = BoxObject(vec2(0), ivec2(32, 16))
	outline.outline = Glyph("#", Colours.WHITE, Colours.BLACK)

	noProgramText = TextObject(vec2(11, 7), ivec2(10, 1))
	noProgramText.text = "No Program"
	noProgramText.fg_colour = Colours.WHITE
	noProgramText.bg_colour = Colours.RED

	return true
end

function Quit()
	outline:release()
	noProgramText:release()

	return true
end
