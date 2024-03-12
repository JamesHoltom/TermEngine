--[[
-- @author James Holtom
--]]

--[[
-- @brief Extends the GameObject usertype to implement drop-down fields.
-- @param _position 	The position of the textbox.
-- @param _size 			The size of the textbox.
--]]
function DropdownControl(_name, _position, _size, _form, _options)
	local self = generateControlSelf("DropdownControl", _name, _position, _size, _form, _options)

	--[[
	-- @brief Checks if the given row/column intersects with the control.
	-- @param _rowcol The row/column to check.
	-- @returns Whether the given row/column is part of the control.
	--]]
	local _isOver = function(_rowcol)
		if self.is_open == true then
			return (
				_rowcol.y == self.label_obj.position.y and _rowcol.x >= self.label_obj.position.x and _rowcol.x < self.label_obj.position.x + self.label_obj.characterMap.size.x
			) or (
				_rowcol >= self.label_obj.position + Ivec2(self.label_size, 1) and _rowcol < self.label_obj.position + Ivec2(self.label_obj.characterMap.size.x, self.list_count + 1)
			)
		else
			return _rowcol >= self.label_obj.position and _rowcol < self.label_obj.position + self.label_obj.characterMap.size
		end
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

	--[[
	-- @brief Sets if the drop-down control is open.
	-- @param _is_open Should the control be open?
	--]]
	local _setIsOpen = function(_is_open)
		self.is_open = _is_open

		for k, v in ipairs(self.options.objects) do
			if k > #self.options.objects then
				break
			end

			v.active = _is_open
		end
	end

	-- @brief Redraws the control after the state has changed.
	local _refreshControl = function()
		local selected_value = ""

		for k, v in ipairs(self.options.objects) do
			local index = k + self.list_offset
			local selected = self.options.selected[index]

			if self.options.selected[k] == true and selected_value == "" then
				selected_value = self.options.labels[k]
			end

			if self.is_open == true then
				local label = self.options.labels[index]
				local text_colour, bg_colour

				if k == 1 and index > 1 then
					label = string.format("%-"..self.value_size.."s%s", label, "^")
				elseif k == self.list_count and index < #self.options.labels then
					label = string.format("%-"..self.value_size.."s%s", label, "v")
				end

				if v.hovering == true then
					text_colour = self.colours.hover_text
					bg_colour = self.colours.hover_bg
				elseif selected == true then
					text_colour = self.colours.row_selected_text
					bg_colour = self.colours.row_selected_bg
				else
					text_colour = self.colours.default_text

					if k % 2 == 0 then
						bg_colour = self.colours.row_primary_bg
					else
						bg_colour = self.colours.row_secondary_bg
					end
				end

				v:set(SetText(v, label, text_colour, bg_colour, true, 2))
			end
		end

		local label_colour, label_bg = self.colours.label_text, self.colours.label_bg
		local dropdownBtn = "v"

		if self.is_open == true then
			dropdownBtn = "^"
		end

		local text_value = string.format("%s%-"..math.max(self.value_size, 2).."s%s", self.label, selected_value, dropdownBtn)

		if self.label_obj.hovering == true then
			label_colour = self.colours.hover_text
			label_bg = self.colours.hover_bg
		elseif self.is_valid == false then
			label_colour = self.colours.error_text
			label_bg = self.colours.error_bg
		end

		self.label_obj:set(SetText(self.label_obj, text_value, label_colour, label_bg, true, 2))
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
				_setIsOpen(not self.is_open)
				_refreshControl()

				return
			end	
			
			if self.is_open == true then
				for k, v in ipairs(self.options.objects) do
					local index = k + self.list_offset

					if v.hovering == true then
						if self.select_multiple == false then
							self.options.selected[index] = true
						else
							self.options.selected[index] = not self.options.selected[index]
						end
					else
						if self.select_multiple == false then
							self.options.selected[index] = false
						end
					end
				end

				_setIsOpen(false)
			end
		end

		_refreshControl()
	end

	--[[
	-- @brief Fires when the user scrolls the mouse while the control is selected.
	-- @param _data Data containing information about the scroll event.
	--]]
	local _doScrollEvents = function(_data)
		if self.is_open == true then
			self.list_offset = self.list_offset - math.floor(_data.movement.y)

			if self.list_offset < 0 then
				self.list_offset = 0
			elseif self.list_offset > #self.options.values - self.list_count then
				self.list_offset = #self.options.values - self.list_count
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

		if _is_selected == false then
			_setIsOpen(false)
		end

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
		elseif key == "list_count" then
			return math.min(self.list_count, #self.options.objects)
		elseif key == "name" or key == "label_size" or key == "is_open" or key == "select_multiple" or key == "required" or key == "validator" then
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
		doScrollEvents = _doScrollEvents,
		refreshControl = _refreshControl,
		onHover = _onHover,
		onSelect = _onSelect
	}, {
		__index    = mtIndex,
		__newindex = mtNewIndex,
		__type			= { name = "DropdownControl" }
	})
end
