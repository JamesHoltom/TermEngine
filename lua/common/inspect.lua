--[[
-- @author James Holtom
--]]

--[[
-- @brief Inspects the contents of a table.
-- @param value The table to inspect.
--]]
function inspect(value)
	if type(value) == 'table' then
		local s = '{\n'

		for k,v in pairs(value) do
			s = s .. '  ['..k..'] = ' .. inspect(v) .. ',\n'
		end

		return s .. '}'
	else
		return tostring(value)
	end
end
