--[[
-- @author James Holtom
--]]

--[[
-- @brief Extends the GameObject usertype to make inputting text in a form simpler.
-- @param _name				The name of the control.
-- @param _position		The position of the control.
-- @param _size 			The size of the control.
-- @param _form				The form object this control is associated with.
-- @param _options		Additional options to configure the control with.
--]]
function TextControl(_name, _position, _size, _form, _options)
	local colours = _options.colours or {}

	local self = {
		name = _name,													-- @brief The name of the control.
		object = GameObject(_position, _size , _form.game_scene or defaultGameScene),
		colours = {
			default_text = colours.default_text or characters.DEFAULT_FOREGROUND_COLOUR,
			default_bg = colours.default_bg or characters.DEFAULT_BACKGROUND_COLOUR,
			selected_text = colours.selected_text or characters.DEFAULT_FOREGROUND_COLOUR,
			selected_bg = colours.selected_bg or Colours.DARK_GREY,
			cursor_text = colours.cursor_text or characters.DEFAULT_BACKGROUND_COLOUR,
			cursor_bg = colours.cursor_bg or characters.DEFAULT_FOREGROUND_COLOUR,
			error_text = colours.error_text or characters.DEFAULT_FOREGROUND_COLOUR,
			error_bg = colours.error_bg or Colours.RED
		},
		value = "",														-- @brief The input value.
		default = "",													-- @brief The default value.
		cursor_at = 1,												-- @brief The index character, relevant to the game object, to show the cursor at.
		cursor_offset = 0,										-- @brief The offset of characters in the input value to start writing to the game object.
		required = _options.required == true,	-- @brief Is it mandatory for text to be written upon submission?
		max_length = false,										-- @brief The maximum length of the input value.
		validator = nil,											-- @brief Function that validates if the written text is valid.
		is_valid = true												-- @brief Flag to indicate if the current input value is valid.
	}
	
	local _setCursor = function(_at)
		local end_index = utf8.len(self.value) + 1
		local end_select = math.max(end_index, self.object.characterMap.size.x)

		if type(_at) == "number" and _at >= 1 and _at <= end_select then
			self.cursor_at = math.min(_at, end_index)
		end
	end

	local _refreshControl = function(_is_selected)
		local text, text_colour, bg_colour

		if self.object.hovering == true then
			text_colour = self.colours.selected_text
			bg_colour = self.colours.selected_bg
		elseif self.is_valid == false then
			text_colour = self.colours.error_text
			bg_colour = self.colours.error_bg
		else
			text_colour = self.colours.default_text
			bg_colour = self.colours.default_bg
		end

		if utf8.len(self.value) > 0 then
			text = self.value:sub(self.cursor_offset)
		else
			text = ""
		end

		self.object:set(SetText(self.object, text, text_colour, bg_colour, true, 2))

		if _is_selected == true then
			self.object.characterMap.data[self.cursor_at].foregroundColour = self.colours.cursor_text
			self.object.characterMap.data[self.cursor_at].backgroundColour = self.colours.cursor_bg
		end
	end

	local _validate = function()
		if self.required == true then
			local ok, error = isRequired(self)

			if ok == false then
				return ok, self.value, error
			end
		end

		if self.validator ~= nil then
			local ok, error = self:validator()

			if ok == false then
				return ok, self.value, error
			end
		end

		self.is_valid = result

		_refreshControl(false)

		return true, self.value, ""
	end

	local _clearValidation = function()
		self.is_valid = true

		_refreshControl(false)
	end

	local _setMaxLength = function(value)
		if (value > 0) then
			self.max_length = value
		else
			self.max_length = false
		end
	end

	local _doMouseEvents = function(_selected, _data)
		if _selected == true then
			textInput.start()

			_setCursor(_data.rowcol.x - self.object.position.x + 1)
		else
			textInput.stop()
		end

		_refreshControl(_selected)
	end

	local _doKeyEvents = function(_data)
		local offset = self.cursor_at + self.cursor_offset
		local char_length = utf8.len(self.value)

		if _data.key == "Left" then
			if self.cursor_at > 1 then
				self.cursor_at = self.cursor_at - 1
			elseif self.cursor_offset > 0 then
				self.cursor_offset = self.cursor_offset - 1
			end
		elseif _data.key == "Right" then
			local limit = char_length + 1

			if self.max_length ~= false then
				limit = self.max_length
			end

			if offset < limit then
				if self.cursor_at < self.object.characterMap.size.x then
					self.cursor_at = self.cursor_at + 1
				else
					self.cursor_offset = self.cursor_offset + 1
				end
			end
		elseif _data.key == "Backspace" and char_length > 0 and offset > 1 then
			if self.cursor_at > 1 then
				self.cursor_at = self.cursor_at - 1
			else
				self.cursor_offset = self.cursor_offset - 1
			end

			offset = offset - 1

			local left, right = utf8.offset(self.value, offset) - 1, utf8.offset(self.value, offset + 1)
			local new_value = ""

			if left ~= nil then
				new_value = self.value:sub(1, left)
			end

			if right ~= nil then
				new_value = new_value .. self.value:sub(right)
			end

			self.value = new_value
			self.is_valid = true
		elseif _data.key == "Delete" and char_length > 0 and offset <= char_length then
			local left, right = utf8.offset(self.value, offset - 1), utf8.offset(self.value, offset + 1)
			local new_value = ""

			if left ~= nil and offset > 1 then
				new_value = self.value:sub(1, left)
			end

			if right ~= nil then
				new_value = new_value .. self.value:sub(right)
			end

			self.value = new_value
			self.is_valid = true
		elseif _data.key == "Home" then
			self.cursor_at = 1
			self.cursor_offset = 0
		elseif _data.key == "End" then
			self.cursor_at = math.min(char_length, self.object.characterMap.size.x) + 1
			self.cursor_offset = math.max(0, char_length - self.object.characterMap.size.x)
		elseif _data.key == "X" and _data.ctrl then
			clipboard.set(self.value)

			self.value = ""
			self.cursor_at = 1
			self.cursor_offset = 0
			self.is_valid = true
		elseif _data.key == "C" and _data.ctrl then
			clipboard.set(self.value)
		elseif _data.key == "V" and _data.ctrl then
			self.value = clipboard.get()
			self.cursor_at = math.min(char_length, self.object.characterMap.size.x) + 1
			self.cursor_offset = math.max(0, char_length - self.object.characterMap.size.x)
			self.is_valid = true
		end

		_refreshControl(true)
	end

	local _doTextEvents = function(_data)
		if self.max_length == false or utf8.len(self.value) < self.max_length then
			local offset = utf8.offset(self.value, self.cursor_at + self.cursor_offset)
			local new_value = ""

			if offset ~= nil then
				new_value = self.value:sub(1, offset - 1) .. new_value .. _data.text .. self.value:sub(offset, -1)
			else
				new_value = _data.text
			end

			self.value = new_value
			self.is_valid = true

			if self.cursor_at < self.object.characterMap.size.x then
				self.cursor_at = self.cursor_at + 1
			else
				self.cursor_offset = self.cursor_offset + 1
			end

			_refreshControl(true)
		end
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
		if key == "id" or key == "position" then
			return self.object[key]
		elseif key == "size" then
			return self.object.characterMap.size
		elseif key == "name" or key == "value" or key == "max_length" or key == "required" or key == "validator" then
			return self[key]
		elseif key == "cursor" then
			return self.cursor_at + self.cursor_offset
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
				self.object.characterMap.size = value
			end
		elseif key == "value" then
			if self.max_length ~= false and utf8.len(value) <= self.max_length then
				self[key] = tostring(value)
			end
		elseif key == "max_length" then
			_setMaxLength(tonumber(value))
		elseif key == "required" then
			self[key] = (value == true)
		elseif key == "cursor" then
			_setCursor(tonumber(value))
		end

		_refreshControl(false)
	end

	_setMaxLength(tonumber(_options.max_length or 0))
	_refreshControl(false)

	return setmetatable({
		validate = _validate,
		clearValidation = _clearValidation,
		doMouseEvents = _doMouseEvents,
		doKeyEvents = _doKeyEvents,
		doTextEvents = _doTextEvents,
		refreshControl = _refreshControl
	}, {
		__index    	= mtIndex,
		__newindex 	= mtNewIndex,
		__type			= { name = "TextControl" }
	})
end
