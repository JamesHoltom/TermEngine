--[[
-- @author James Holtom
--]]

--[[
-- @brief Extends the GameObject usertype to make checkbox fields.
-- @param _position 	The position of the textbox.
-- @param _size 			The size of the textbox.
--]]
function CheckboxControl(_name, _position, _size, _form, _options)
	local self = generateControlSelf("CheckboxControl", _name, _position, _size, _form, _options)

	--[[
	-- @brief Checks if the given row/column intersects with the control.
	-- @param _rowcol The row/column to check.
	-- @returns Whether the given row/column is part of the control.
	--]]
	local _isOver = function(_rowcol)
		return _rowcol >= self.label_obj.position and _rowcol < self.label_obj.position + Ivec2(self.value_size, #self.options.objects + 1)
	end

	--[[
	-- @brief Returns a list of the selected values.
	-- @returns A list of selected values.
	--]]
	local _getValues = function()
		local values = {}

		for k, v in ipairs(self.options.values) do
			if self.options.selected[k] == true then
				table.insert(values, v)
			end
		end

		return values
	end

	-- @brief Redraws the control after the state has changed.
	local _refreshControl = function()
		local label_colour, label_bg = self.colours.label_text, self.colours.default_bg

		if self.label_obj.hovering == true then
			label_bg = self.colours.hover_bg
		end

		self.label_obj:set(SetText(self.label_obj, self.label, label_colour, label_bg, true, 2))

		for k, v in ipairs(self.options.objects) do
			local label, selected = self.options.labels[k], self.options.selected[k]
			local text, text_colour, bg_colour

			if selected == true then
				text = self.value_texts.checked
				text_colour = self.colours.checked_text
			else
				text = self.value_texts.unchecked
				text_colour = self.colours.default_text
			end

			if v.hovering == true then
				bg_colour = self.colours.hover_bg
			elseif self.is_valid == false then
				bg_colour = self.colours.error_bg
			else
				bg_colour = self.colours.default_bg
			end

			v:set(SetText(v, label .. text, text_colour, bg_colour, true, 2))
		end
	end

	--[[
	-- @brief Validates the control. If the current value is invalid, the control is put into an error state.
	-- @returns The result of the validation, the value and a message if the value is invalid.
	--]]
	local _validate = function()
		local result, data, error = true, _getValues(), ""

		if self.required == true then
			if #data == 0 then
				result, error = false, "This field is required!"
			end
		end

		if self.validator ~= nil and result ~= false then
			result, error = self:validator()
		end

		self.is_valid = result
		_refreshControl()

		return result, data, error
	end

	-- @brief Resets the control to the default state.
	local _clearValidation = function()
		self.is_valid = true

		_refreshControl()
	end

	--[[
	-- @brief Fires when the user clicks on the control.
	-- @param _data Data containing information about the mouse event.
	--]]
	local _doMouseEvents = function(_data)
		if self.is_selected == true then
			if self.label_obj.hovering == true then
				return
			end
			
			for k, v in ipairs(self.options.objects) do
				if v.hovering == true then
					if self.select_multiple == false then
						self.options.selected[k] = true
					else
						self.options.selected[k] = not self.options.selected[k]
					end
				else
					if self.select_multiple == false then
						self.options.selected[k] = false
					end
				end
			end
		end

		_refreshControl()
	end

	--[[
	-- @brief Fires when the user's mouse hovers over/out of the control.
	-- @param _is_hovering Is the user's mouse hovering over this control?
	--]]
	local _onHover = function(_is_hovering)
		self.is_hovering = _is_hovering

		_refreshControl()
	end

	--[[
	-- @brief Fires when the user (de)selects the control.
	-- @param _is_hovering Has the user selected this control?
	--]]
	local _onSelect = function(_is_selected)
		self.is_selected = _is_selected

		_refreshControl()
	end

	-- @brief Cleans up the object after use.
	local _release = function(_)
		self.label_obj:release()

		for _, v in ipairs(self.options.objects) do
			v:release()
		end
	end

	--[[
	-- @brief Configures the metatable for getting properties.
	-- @param key The name of the property to retrieve.
	-- @returns The value of the property.
	--]]
	local mtIndex = function(_, key)
		if key == "labels" or key == "values" or key == "selected" then
			return self.options[key]
		elseif key == "label_id" then
			return self.label_obj.id
		elseif key == "ids" then
			local ids = {}

			for _, v in ipairs(self.options.objects) do
				table.insert(ids, v.id)
			end

			return ids
		elseif key == "value" then
			return _getValues()
		elseif key == "value_count" then
			return #self.options.values
		elseif key == "name" or key == "required" or key == "validator" then
			return self[key]
		elseif key == "position" or key == "active" then
			return self.label_obj[key]
		elseif key == "size" then
			return Ivec2(self.label_size + self.value_size, 1)
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
		if key == "position" and value.__type.name == "Ivec2" then
			self.label_obj.position = Ivec2(value)

			for k, v in ipairs(self.options.objects) do
				v.position = Ivec2(self.label_obj.position + Ivec2(0, k), 1)
			end
		elseif key == "size" and type(value) == "number" and value >= 1 then
			self.value_size = value

			for _, v in ipairs(self.options.objects) do
				v.characterMap.size = Ivec2(self.value_size, 1)
			end
		elseif key == "label" and type(value) == "string" then
			self.label = value
			self.label_size = utf8.len(value)
			self.label_obj.characterMap.size = Ivec2(self.label_size, 1)
		elseif key == "required" then
			self.value = (value == true)
		elseif key == "validator" then
			if type(v) == "function" then
				self.validator = value
			end
		end
	end

	_refreshControl()

	return setmetatable({
		release = _release,
		validate = _validate,
		clearValidation = _clearValidation,
		isOver = _isOver,
		doMouseEvents = _doMouseEvents,
		refreshControl = _refreshControl,
		onHover = _onHover,
		onSelect = _onSelect
	}, {
		__index    = mtIndex,
		__newindex = mtNewIndex,
		__type			= { name = "CheckboxControl" }
	})
end
