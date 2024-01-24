--[[
-- @author James Holtom
--]]

--[[
-- @brief Extends the GameObject usertype to make inputting numbers simpler.
-- @param _name				The name of the control.
-- @param _position		The position of the control.
-- @param _size 			The size of the control.
-- @param _form				The form object this control is associated with.
-- @param _options		Additional options to configure the control with.
--]]
function NumberControl(_name, _position, _size, _form, _options)
	local self = generateControlSelf("NumberControl", _name, _position, _size, _form, _options)

	local _setCursor = function(_at)
		local end_index = #self.text_value + 1
		local end_select = math.max(end_index, self.value_size)
		local at = tonumber(_at) - self.label_size

		if at >= 1 and at <= end_select then
			self.cursor_at = self.label_size + math.min(at, end_index)
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

	local _refreshControl = function()
		local text_value, text_colour, bg_colour

		if self.is_selected == true and textInput.isActive() == true then
			text_value = self.text_value
		else
			text_value = tostring(self.value)
		end

		text_value = text_value:sub(self.cursor_offset)

		if self.enable_step == true then
			text_value = string.format("%s%"..math.max(self.value_size - #text_value, 2).."s", text_value, "<>")
		end

		if self.is_hovering == true then
			text_colour = self.colours.hover_text
			bg_colour = self.colours.hover_bg
		elseif self.is_valid == false then
			text_colour = self.colours.error_text
			bg_colour = self.colours.error_bg
		else
			text_colour = self.colours.default_text
			bg_colour = self.colours.default_bg
		end

		self.object:set(SetText(self.object, self.label .. text_value, text_colour, bg_colour, true, 2))

		if self.is_selected == true then
			self.object.characterMap.data[self.cursor_at].foregroundColour = self.colours.cursor_text
			self.object.characterMap.data[self.cursor_at].backgroundColour = self.colours.cursor_bg
		end
	end

	local _validate = function()
		local result, error = true, ""

		if self.validator ~= nil and result ~= false then
			result, error = self:validator()
		end

		self.is_valid = result
		_refreshControl()

		return result, self.value, error
	end

	local _clearValidation = function()
		self.is_valid = true

		_refreshControl()
	end

	local _doMouseEvents = function(_data)
		local cursor = _data.rowcol.x - self.object.position.x
		local step_down_hover = self.enable_step and (cursor == (self.label_size + self.value_size - 2))
		local step_up_hover = self.enable_step and (cursor == (self.label_size + self.value_size - 1))
		local value = tonumber(self.text_value) or 0

		if self.is_hovering == true then
			if step_down_hover == true and self.minimum ~= false then
				self.text_value = tostring(math.max(value - self.step_value, self.minimum))
			elseif step_up_hover == true and self.maximum ~= false then
				self.text_value = tostring(math.min(value + self.step_value, self.maximum))
			else
				_setCursor(cursor + 1)
			end
		end

		_refreshControl()
	end

	local _doKeyEvents = function(_data)
		local cursor_at = self.cursor_at - self.label_size
		local offset = cursor_at + self.cursor_offset
		local char_length = #self.text_value

		if _data.key == "Left" then
			if cursor_at > 1 then
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
			if cursor_at > 1 then
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
			self.cursor_at = self.label_size + 1
			self.cursor_offset = 0
		elseif _data.key == "End" then
			self.cursor_at = math.min(self.label_size + char_length, self.object.characterMap.size.x) + 1
			self.cursor_offset = math.max(0, self.label_size + char_length - self.object.characterMap.size.x)
		elseif _data.key == "X" and _data.ctrl then
			clipboard.set(self.text_value)

			self.text_value = ""
			self.value = 0
			self.cursor_at = self.label_size + 1
			self.cursor_offset = 0
		elseif _data.key == "C" and _data.ctrl then
			clipboard.set(self.text_value)
		elseif _data.key == "V" and _data.ctrl then
			self.text_value = clipboard.get()
			self.cursor_at = math.min(self.label_size + char_length, self.object.characterMap.size.x) + 1
			self.cursor_offset = math.max(0, self.label_size + char_length - self.object.characterMap.size.x)
		end

		_refreshControl()
	end

	local _doTextEvents = function(_data)
		local offset = self.cursor_at - self.label_size + self.cursor_offset

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
		
		_refreshControl()
	end

	local _onHover = function(_is_hovering)
		self.is_hovering = _is_hovering

		_refreshControl()
	end

	local _onSelect = function(_is_selected)
		self.is_selected = _is_selected

		if _is_selected == true and textInput.isActive() == false then
			self.text_value = tostring(self.value)
			textInput.start()
		elseif _is_selected == false and textInput.isActive() == true then
			_setValue()
			textInput.stop()
		end

		_refreshControl()
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
			return Ivec2(self.label_size + self.value_size, 1)
		elseif key == "name" or key == "value" or key == "minimum" or key == "maximum" or key == "validator" or key == "enable_step" or key == "step_value" then
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
				self.value_size = tonumber(value)
				self.object.characterMap.size = Ivec2(self.label_size + self.value_size, 1)
			end
		elseif key == "value" then
			self.text_value = tostring(value)
			_setValue()
		elseif key == "minimum" or key == "maximum" or key == "step_value" then
			self[key] = tonumber(value)
		elseif key == "enable_step" then
			self[key] = (value == true)
		elseif key == "validator" then
			if type(v) == "function" then
				self.validator = value
			end
		elseif key == "cursor" then
			_setCursor(tonumber(value))
		end

		_refreshControl()
	end

	local value = tonumber(_options.value)

	if value ~= nil then
		self.value = value
		_setValue()
	end

	_setCursor(self.label_size + 1)
	_refreshControl()

	return setmetatable({
		release = _release,
		validate = _validate,
		clearValidation = _clearValidation,
		doMouseEvents = _doMouseEvents,
		doKeyEvents = _doKeyEvents,
		doTextEvents = _doTextEvents,
		refreshControl = _refreshControl,
		onHover = _onHover,
		onSelect = _onSelect
	}, {
		__index    	= mtIndex,
		__newindex 	= mtNewIndex,
		__type			= { name = "NumberControl" }
	})
end
