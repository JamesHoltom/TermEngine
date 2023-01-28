--[[
-- @author James Holtom
--]]

local outline, noProgramText

function Init()
	print(tostring(defaultScene().charmap.size))
	outline = BoxObject(defaultScene(), Values.IVEC2_ZERO, defaultScene().charmap.size)
	outline.outline = Character("#", Colours.WHITE, Colours.GREEN)

	noProgramText = TextObject(defaultScene(), ivec2(11, 7), ivec2(10, 1))
	noProgramText.text = "No Program"
	noProgramText.fg_colour = Colours.WHITE
	noProgramText.bg_colour = Colours.RED

	return true
end
