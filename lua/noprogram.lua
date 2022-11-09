--[[
-- @author James Holtom
--]]

local outline, noProgramText

function Init()
	outline = BoxObject(Values.IVEC2_ZERO, view.getSize())
	outline.outline = Character("#", Colours.WHITE, Colours.BLACK)

	noProgramText = TextObject(ivec2(11, 7), ivec2(10, 1))
	noProgramText.text = "No Program"
	noProgramText.fg_colour = Colours.WHITE
	noProgramText.bg_colour = Colours.RED

	return true
end
