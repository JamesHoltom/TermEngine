--[[
-- @author James Holtom
--]]

local menuObj

function Init()
	menuObj = MenuObject(Values.IVEC2_ZERO)
	menuObj.addOptions(
		{ title = "Test", size = Ivec2(8, 1), callback = function() print("TEST!") end },
		{ title = "Hello\nWorld", size = Ivec2(8, 2), callback = function() print("HELLO!") end },
		{ title = "A", new_line = false, size = Ivec2(4, 1), callback = function() print("A!") end },
		{ title = "B", offset = 4, size = Ivec2(4, 1), callback = function() print("B!") end },
		{ title = "Temp", size = Ivec2(8, 1), callback = function() print("TEMP!") end })

	return true
end

function Loop(_)
	if keyboard.isPressed("R") then
		menuObj.use_keys = not menuObj.use_keys

		print("Keyboard selection is "..((menuObj.use_keys and "on") or "off"))
	end
end
