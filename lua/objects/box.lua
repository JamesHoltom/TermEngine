--[[
-- @author James Holtom
--]]

local empty_character = Character(characters.NO_CHARACTER, characters.DEFAULT_FOREGROUND_COLOUR, characters.DEFAULT_BACKGROUND_COLOUR)

--[[
-- @brief Extends the GameObject usertype to make drawing boxes simpler.
-- @param _position 	The position of the box.
-- @param _size 			The size of the box.
-- @param _game_scene The game scene to add the box to.
--]]
function BoxObject(_position, _size, _game_scene)
	local self = {
		object = GameObject(_position, _size, _game_scene or defaultGameScene),	-- @brief Handle to the Object.
		fill = empty_character,																									-- @brief The Character to use for the fill.
		outline = empty_character,																							-- @brief The Character to use for the outline.
		has_outline = false																											-- @brief Has an outline been set?
	}
	
	-- @brief Refreshes the object data with the updated settings.
	local _setData = function()
		local minW = self.object.characterMap.size.x
		local maxW = (self.object.characterMap.size.x * self.object.characterMap.size.y) - minW
		
		self.object:set(function(_, index)
			if self.has_outline and (index < minW or index > maxW or math.fmod(index, minW) <= 1) then
				return self.outline
			else
				return self.fill
			end
		end)
	end
	
	--[[
	-- @brief Does the box have an outline?
	-- @returns If the box have an outline.
	--]]
	local _hasOutline = function()
		return self.has_outline
	end
	
	-- @brief Removes the outline from the box.
	local _unsetOutline = function()
		self.outline = empty_character
		self.has_outline = false
		
		_setData()
	end

	-- @brief Cleans up the object after use.
	local _release = function(_)
		self.object:release()
	end

	--[[
	-- @brief Configures the metatable for getting properties.
	-- @param key The name of the property to retrieve.
	-- @returns The value of the property.
	--]]
	local mtIndex = function(_, key)
		if key == "id" or key == "position" or key == "active" then
			return self.object[key]
		elseif key == "size" then
			return self.object.characterMap.size
		elseif key == "fill" or key == "outline" then
			return self[key]
		else
			return nil
		end
	end

	--[[
	-- @brief Configures the metatable for setting properties.
	-- @param key		The name of the property to set.
	-- @param value The value to set the property to.
	--]]
	local mtNewIndex = function(_, key, value)
		if key == "position" then
			if value.__type.name == "Ivec2" then
				self.object.position = Ivec2(value)
			end
		elseif key == "size" then
			if value.__type.name == "Ivec2" and value >= Values.IVEC2_ONE then
				self.object.characterMap.size = Ivec2(value)
				_setData()
			end
		elseif key == "active" then
			self.object.active = value == true
		elseif key == "fill" then
			if value.__type.name == "Character" then
				self[key] = value
				
				_setData()
			end
		elseif key == "outline" then
			if value.__type.name == "Character" then
				self[key] = value

				if value.character == characters.NO_CHARACTER then
					self.has_outline = false
				else
					self.has_outline = true
				end
				
				_setData()
			end
		end
	end
	
	return setmetatable({
		hasOutline   	= _hasOutline,
		unsetOutline 	= _unsetOutline,
		release      	= _release
	}, {
		__index    		= mtIndex,
		__newindex 		= mtNewIndex,
    __type      	= { name = "LineObject" }
	})
end
