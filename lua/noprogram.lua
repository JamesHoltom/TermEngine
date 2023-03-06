--[[
-- @author James Holtom
--]]

local outline, noProgramText

function Init()
	outline = BoxObject(Values.IVEC2_ZERO, defaultScene.charmap.size)
	outline.outline = Character("#", Colours.WHITE, Colours.BLACK)

	noProgramText = TextObject(ivec2(11, 7), ivec2(10, 2))
	noProgramText.fg_colour = Colours.WHITE
	noProgramText.bg_colour = Colours.RED

	return true
end

function Loop()
	noProgramText.text = "No Program0123456789"
end