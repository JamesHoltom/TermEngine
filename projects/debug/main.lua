--[[
-- @author James Holtom
--]]

local debugText

function Init()
	debugText = TextObject(Values.IVEC2_ZERO, ivec2(32, 16))

	return true
end

function Loop(_)
	debugText.text = getDebugInfo()
end
