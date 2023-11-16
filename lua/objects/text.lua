--[[
-- @author James Holtom
--]]

--[[
-- @brief Extends the GameObject usertype to make writing text simpler.
-- @param _position 	The position of the textbox.
-- @param _size 			The size of the textbox.
-- @param _text 			The text to write.
-- @param _game_scene The game scene to add the object to.
--]]
function TextObject(_position, _size, _text, _game_scene)
	local self = {
		object = GameObject(_position, _size, _game_scene or "default"),	-- @brief Handle to the Object.
		text = tostring(_text or ""),																			-- @brief The text to render.
		fit_text = false,																									-- @brief Should the background colour fit the text, or the Object bounds?
		tab_size = 2,																											-- @brief The size of tab characters (i.e. '\t').
		fg_colour = characters.DEFAULT_FOREGROUND_COLOUR,									-- @brief The text colour.
		bg_colour = characters.DEFAULT_BACKGROUND_COLOUR									-- @brief The background colour.
	}

	-- @brief Refreshes the object data with the updated settings.
	local _setData = function()
		self.object:set(SetText(self.object, self.text, self.fg_colour, self.bg_colour, self.fit_text, self.tab_size))
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
			return self.object.data.size
		elseif key == "text" or key == "fit_text" or key == "tab_size" or key == "fg_colour" or key == "bg_colour" then
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
				self.object.data.size = value
				_setData()
			end
		elseif key == "active" then
			self.object.active = value == true
		elseif key == "text" then
			self.text = tostring(value)
			_setData()
		elseif key == "fit_text" then
			self.fit_text = value == true
			_setData()
		elseif key == "tab_size" then
			self.tab_size = value
			_setData()
		elseif key == "fg_colour" or key == "bg_colour" then
			if value.__type.name == "Vec4" and value:clamp(0, 255) == value then
				self[key] = Vec4(value)
				_setData()
			end
		end
	end

	_setData()

	return setmetatable({
		release = _release
	}, {
		__index    = mtIndex,
		__newindex = mtNewIndex
	})
end
