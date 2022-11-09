--[[
-- @author James Holtom
--]]

local empty_character = Character(characters.NO_CHARACTER, characters.DEFAULT_FOREGROUND_COLOUR, characters.DEFAULT_BACKGROUND_COLOUR)

--[[
-- @brief Extends the Object usertype to make drawing boxes simpler.
-- @param _position The position of the box.
-- @param _size The size of the box.
--]]
function BoxObject(_position, _size)
	local self = {
		obj = Object(_position, _size),	-- @brief Handle to the Object.
		fill = empty_character,					-- @brief The Character to use for the fill.
		outline = empty_character,			-- @brief The Character to use for the outline.
		has_outline = false							-- @brief Has an outline been set?
	}
	
	-- @brief Refreshes the object data with the updated settings.
	local _setData = function()
		local w = self.obj.size.x
		
		self.obj:set(function(data, index, _)
			if self.has_outline and (index < w or index > (#self.obj.data - w) or math.fmod(index, w) <= 1) then
				data[index] = self.outline
			else
				data[index] = self.fill
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
		self.obj:release()
	end

	--[[
	-- @brief Configures the metatable for getting properties.
	-- @param key The name of the property to retrieve.
	-- @returns The value of the property.
	--]]
	local mtIndex = function(_, key)
		if key == "position" or key == "size" or key == "active" then
			return self.obj[key]
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
			self.obj.position = ivec2(value)
		elseif key == "size" then
			self.obj.size = ivec2(value)
			_setData()
		elseif key == "active" then
			self.obj.active = value
		elseif key == "fill" then
			self[key] = Character(value)
			
			_setData()
		elseif key == "outline" then
			self[key] = Character(value)

			if value.character == characters.NO_CHARACTER then
				self.has_outline = false
			else
				self.has_outline = true
			end
			
			_setData()
		end
	end
	
	return setmetatable({
		hasOutline   = _hasOutline,
		unsetOutline = _unsetOutline,
		release      = _release
	}, {
		__index    = mtIndex,
		__newindex = mtNewIndex
	})
end
