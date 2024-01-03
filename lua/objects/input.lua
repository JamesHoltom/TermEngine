--[[
-- @author James Holtom
--]]

--[[
-- @brief Extends the GameObject usertype to make writing text simpler.
-- @param _position		The position of the textbox.
-- @param _size 			The size of the textbox.
-- @param _game_scene The game scene to add the textbox to.
--]]
function InputObject(_position, _size, _game_scene)
	local self = {
		object = GameObject(_position, _size, _game_scene or defaultGameScene),	-- @brief Handle to the Object.
		name = _name,																														-- @brief The name of the input.
		label = "",																															-- @brief The input label.
		value = "",																															-- @brief The input value.
		max_length = _size.x * _size.y,																					-- @brief The maximum length of the value.
		cursor = 1																															-- @brief The position of the text cursor.
	}

	--[[
	-- @brief Sets the text cursor at the given position
	-- @param _at The index position to set the cursor to.
	--]]
	local _setCursor = function(_at)
		if _at > 0 and _at <= math.min(#self.value + 1, self.max_length) then
			self.cursor = _at
		end
	end

	-- @brief Refreshes the object data with the updated settings.
	local _setData = function()
		local is_value = #self.value > 0
		local text, text_colour, bg_colour

		if is_value == true then
			text = self.value
			text_colour = characters.DEFAULT_FOREGROUND_COLOUR
		else
			text = self.label
			text_colour = Colours.LIGHT_GREY
		end
	
		if self.object.hovering == true then
			bg_colour = Colours.DARK_GREY
		else
			bg_colour = characters.DEFAULT_BACKGROUND_COLOUR
		end

		self.object:set(SetText(self, text, text_colour, bg_colour, false, 2))

		if GetSelectedObject() == self.object.id then
			self.object.characterMap.data[self.cursor].foregroundColour = characters.DEFAULT_BACKGROUND_COLOUR
			self.object.characterMap.data[self.cursor].backgroundColour = characters.DEFAULT_FOREGROUND_COLOUR
		end
	end

	--[[
	-- @brief Updates the object with data from `object_hover` events.
	-- @param _event The event data.
	--]]
	local _doHoverEvent = function(_event)
		_setData()
	end

	--[[
	-- @brief Updates the object with data from `mouse_down` events.
	-- @param _event The event data.
	--]]
	local _doClickEvent = function(_event)
		local cursor = getIndexFromRowCol(self.object, _event.rowcol - self.object.position)

		_setCursor(cursor)
		_setData()
	end

	--[[
	-- @brief Updates the object with data from `key_down` events.
	-- @param _event The event data.
	--]]
	local _doKeyEvent = function(_event)
		if self.object.id ~= GetSelectedObject() then
			return
		end
		
		if _event.key == "Left" then
			if self.cursor > 1 then
				self.cursor = self.cursor - 1
			end
		elseif _event.key == "Right" then
			if self.cursor < math.min(#self.value + 1, self.max_length) then
				self.cursor = self.cursor + 1
			end
		elseif _event.key == "Up" then
			if self.cursor - self.object.size.x > 0 then
				self.cursor = self.cursor - self.object.size.x
			end
		elseif _event.key == "Down" then
			if self.cursor + self.object.size.x < self.max_length then
				self.cursor = math.min(self.cursor + self.object.size.x, #self.value + 1)
			end
		elseif _event.key == "Backspace" then
			if #self.value > 0 and self.cursor > 1 then
				local offset = utf8.offset(self.value, self.cursor - 1)

				if offset then
					self.cursor = self.cursor - 1
					self.value = self.value:sub(1, offset - 1) .. self.value:sub(offset + 1, -1)
				end
			end
		elseif _event.key == "Delete" then
			if #self.value > 0 and self.cursor < math.min(#self.value + 1, self.max_length) then
				local offset = utf8.offset(self.value, self.cursor)

				if offset then
					self.value = self.value:sub(1, offset - 1) .. self.value:sub(offset + 1, -1)
				end
			end
		elseif _event.key == "Home" then
			self.cursor = 1
		elseif _event.key == "End" then
			self.cursor = math.min(#self.value + 1, self.max_length)
		elseif _event.key == "C" and _event.ctrl then
			clipboard.set(self.value)
		elseif _event.key == "V" and _event.ctrl then
			self.value = string.sub(clipboard.get(), 1, self.max_length)
		end

		_setData()
	end

	--[[
	-- @brief Updates the object with data from `text_input` events.
	-- @param _event The event data.
	--]]
	local _doTextEvent = function(_event)
		if self.object.id ~= GetSelectedObject() then
			return
		end
		
		if #self.value < self.max_length then
			local offset = utf8.offset(self.value, self.cursor)

			if offset then
				self.value = self.value:sub(1, offset - 1) .. _event.text .. self.value:sub(offset, -1)
			end

			if self.cursor < self.max_length then
				self.cursor = self.cursor + 1
			end
			
			_setData(true)
		end
	end

	-- @brief Cleans up the object after use.
	local _release = function()
		UnsetInteractive(self)
		self.object:release()
	end

	--[[
	-- @brief Configures the metatable for getting properties.
	-- @param key The name of the property to retrieve.
	-- @returns The value of the property.
	--]]
	local mtIndex = function(_, key)
		if key == "id" or key == "position" or key == "hovering" or key == "active" then
			return self.object[key]
		elseif key == "size" then
			return self.object.characterMap.size
		elseif key == "label" or key == "value" or key == "cursor" then
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
				self.max_length = self.object.characterMap.size.x * self.object.characterMap.size.y
			end
		elseif key == "active" then
			self.object.active = value == true
		elseif key == "value" or key == "label" then
			self[key] = string.sub(tostring(value), 1, self.max_length)
		elseif key == "cursor" then
			_setCursor(value)
		end
		
		_setData()	
	end

	return SetInteractive(setmetatable({
		doHoverEvent = _doHoverEvent,
		doClickEvent = _doClickEvent,
		doKeyEvent = _doKeyEvent,
		doTextEvent = _doTextEvent,
		release = _release
	}, {
		__index    	= mtIndex,
		__newindex 	= mtNewIndex,
		__type			= { name = "InputControl" }
	}))
end
