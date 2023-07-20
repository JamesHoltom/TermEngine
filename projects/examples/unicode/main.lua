--[[
-- @author James Holtom
--]]

local unicodeText

function Init()
	unicodeText = TextObject(Values.IVEC2_ONE, Ivec2(21, 4), "äëïöüÄËÏÖÜâêîôûÂÊÎÔÛ áéíóúÁÉÍÓÚàèìòùÀÈÌÒÙ ąęįǫųĄĘĮǪŲã ĩõũÃ ĨÕŨ ¿¡‽≤≥≠⋄⌷∅♯")

	return true
end
