--[[
-- @author James Holtom
--]]

--[[
-- @brief Extends the GameObject usertype to make inputting numbers simpler.
-- @param _size 			The size of the number box.
--]]
function NumberControl(_name, _position, _width, _form, _options)
	local colours = _options.colours or {}

	local self = {
		name = _name,
		width = tonumber(_width),
		object = GameObject(_position, Ivec2(tonumber(_width), 1), _form.game_scene or defaultGameScene),
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
		value = 0,								-- @brief The input value.
		text_value = "",
		default = 0,
		cursor_at = 1,
		cursor_offset = 0,
		required = _options.required == true,								-- @brief Is it mandatory for the value to be submitted?
		maximum = tonumber(_options.maximum) or false,
		minimum = tonumber(_options.minimum) or false,
		validator = nil,	-- @brief Function that validates if the number input is valid.
		is_valid = true,
		enable_step = _options.enable_step == true,
		step_value = tonumber(_options.step_value) or 1
	}
	
	local _setCursor = function(_at)
		local end_index = #self.text_value + 1
		local end_select = math.max(end_index, self.object.characterMap.size.x)

		if type(_at) == "number" and _at >= 1 and _at <= end_select then
			self.cursor_at = math.min(_at, end_index)
		end
	end

	local _setValue = function()
		local new_value = tonumber(self.text_value)

		if new_value ~= nil then
			if self.minimum ~= false and new_value < self.minimum then
				self.value = self.minimum
			elseif self.maximum ~= false and new_value > self.maximum then
				self.value = self.maximum
			else
				self.value = new_value
			end
		end

		self.text_value = ""
	end

	local _refreshControl = function(_is_selected)
		local text_value, text_colour, bg_colour

		if textInput.isActive() then
			text_value = self.text_value
		else
			text_value = tostring(self.value)
		end

		text_value = text_value:sub(self.cursor_offset)

		if self.enable_step == true then
			text_value = string.format("%s%"..math.max(self.width - #text_value, 2).."s", text_value, "<>")
		end

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

		self.object:set(SetText(self.object, text_value, text_colour, bg_colour, true, 2))

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

	local _doMouseEvents = function(_selected, _data)
		local cursor = _data.rowcol - self.object.position
		local step_down_hover = self.enable_step and (cursor.x == self.width - 2)
		local step_up_hover = self.enable_step and (cursor.x == self.width - 1)

		if _selected == true then
			if step_down_hover == true and self.minimum ~= false then
				self.value = math.max(self.value - self.step_value, self.minimum)
			elseif step_up_hover == true and self.maximum ~= false then
				self.value = math.min(self.value + self.step_value, self.maximum)
			else
				if not textInput.isActive() then
					self.text_value = tostring(self.value)
					textInput.start()
				end
				
				_setCursor(_data.rowcol.x - self.object.position.x + 1)
			end
		else
			_setValue()

			textInput.stop()
		end

		_refreshControl(_selected)
	end

	local _doKeyEvents = function(_data)
		local offset = self.cursor_at + self.cursor_offset
		local char_length = #self.text_value

		if _data.key == "Left" then
			if self.cursor_at > 1 then
				self.cursor_at = self.cursor_at - 1
			elseif self.cursor_offset > 0 then
				self.cursor_offset = self.cursor_offset - 1
			end
		elseif _data.key == "Right" then
			local limit = char_length + 1

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
			self.text_value = self.text_value:sub(1, offset - 1) .. self.text_value:sub(offset + 1, -1)
			self.is_valid = true
		elseif _data.key == "Delete" and char_length > 0 and offset <= char_length then
			local new_value = ""

			if offset > 1 then
				new_value = self.text_value:sub(1, offset - 1)
			end

			new_value = new_value .. self.text_value:sub(offset + 1, -1)

			self.text_value = new_value
			self.is_valid = true
		elseif _data.key == "Home" then
			self.cursor_at = 1
			self.cursor_offset = 0
		elseif _data.key == "End" then
			self.cursor_at = math.min(char_length, self.object.characterMap.size.x) + 1
			self.cursor_offset = math.max(0, char_length - self.object.characterMap.size.x)
		elseif _data.key == "X" and _data.ctrl then
			clipboard.set(self.text_value)

			self.text_value = ""
			self.value = 0
			self.cursor_at = 1
			self.cursor_offset = 0
		elseif _data.key == "C" and _data.ctrl then
			clipboard.set(self.text_value)
		elseif _data.key == "V" and _data.ctrl then
			self.text_value = clipboard.get()
			self.cursor_at = math.min(char_length, self.object.characterMap.size.x) + 1
			self.cursor_offset = math.max(0, char_length - self.object.characterMap.size.x)
		end

		_refreshControl(true)
	end

	local _doTextEvents = function(_data)
		local offset = self.cursor_at + self.cursor_offset

		if _data.text == "-" then
			if offset > 1 or self.text_value:sub(0, 1) == "-" then
				return
			end
		elseif _data.text == "." then
			if self.text_value:find("%.") ~= nil then
				return
			end
		elseif tonumber(_data.text) == nil then
			return
		else
			if offset == 1 and self.text_value:sub(0, 1) == "-" then
				return
			end
		end

		self.text_value = self.text_value:sub(1, offset - 1) .. _data.text .. self.text_value:sub(offset, -1)

		if self.cursor_at < self.object.characterMap.size.x then
			self.cursor_at = self.cursor_at + 1
		else
			self.cursor_offset = self.cursor_offset + 1
		end
		
		_refreshControl(true)
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
			return Ivec2(self.width, 1)
		elseif key == "name" or key == "width" or key == "value" or key == "minimum" or key == "maximum" or key == "required" or key == "validator" or key == "enable_step" or key == "step_value" then
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
			if value.__type.name == "Ivec2" then
				self.width = tonumber(value.x)
				self.object.characterMap.size = Ivec2(self.width, 1)
			end
		elseif key == "width" then
			self[key] = tonumber(value)
			self.object.characterMap.size = Ivec2(self.width, 1)
		elseif key == "value" then
			self.text_value = tostring(value)
			_setValue()
		elseif key == "minimum" or key == "maximum" or key == "step_value" then
			self[key] = tonumber(value)
		elseif key == "required" or key == "enable_step" then
			self[key] = (value == true)
		elseif key == "cursor" then
			_setCursor(tonumber(value))
		end

		_refreshControl(false)
	end

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
		__type			= { name = "NumberControl" }
	})
end
