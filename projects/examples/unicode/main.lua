--[[
-- @author James Holtom
--]]

local unicodeText

function Init()
	unicodeText = TextObject(Values.IVEC2_ONE, Ivec2(22, 6), "Here is some Unicode!\näëïöüÄËÏÖÜâêîôûÂÊÎÔÛ\náéíóúÁÉÍÓÚàèìòùÀÈÌÒÙ\nąęįǫųĄĘĮǪŲã ĩõũÃ ĨÕŨ\n¿¡‽≤≥≠⋄⌷∅♯")

	return true
end
