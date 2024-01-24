--[[
-- @author James Holtom
--]]

--[[
-- @brief Extends the GameObject usertype to make checkbox fields.
-- @param _position 	The position of the textbox.
-- @param _size 			The size of the textbox.
--]]
function CheckboxControl(_name, _position, _form, _options)
	local self = generateControlSelf("CheckboxControl", _name, _position, 1, _form, _options)

	local _getValues = function()
		local values = {}

		for k, v in ipairs(self.options.values) do
			if self.options.selected[k] == true then
				table.insert(values, v)
			end
		end

		return values
	end

	local _refreshControl = function()
		for k, v in ipairs(self.options.objects) do
			local label, selected = self.options.labels[k], self.options.selected[k]
			local text, text_colour, bg_colour

			if selected == true then
				text = self.value_texts.checked
				text_colour = self.colours.checked_text
			else
				text = self.value_texts.unchecked
				text_colour = self.colours.unchecked_text
			end

			if v.hovering == true then
				bg_colour = self.colours.selected_bg
			elseif self.is_valid == false then
				bg_colour = self.colours.error_bg
			else
				bg_colour = self.colours.default_bg
			end

			v:set(SetText(v, label .. text, text_colour, bg_colour, true, 2))
		end
	end

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

	local _clearValidation = function()
		self.is_valid = true

		_refreshControl()
	end

	local _doMouseEvents = function(_data)
		if self.is_selected == true then
			for k, v in ipairs(self.options.objects) do
				if v.hovering == true then
					self.options.selected[k] = not self.options.selected[k]
					break
				end
			end
		end

		_refreshControl()
	end

	local _onHover = function(_is_hovering)
		self.is_hovering = _is_hovering

		_refreshControl()
	end

	local _onSelect = function(_is_selected)
		self.is_selected = _is_selected

		_refreshControl()
	end

	-- @brief Cleans up the object after use.
	local _release = function(_)
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
		elseif key == "ids" then
			local ids = {}

			for _, v in ipairs(self.options.objects) do
				table.insert(ids, v.id)
			end

			return ids
		elseif key == "value" then
			return _getValues()
		elseif key == "name" or key == "position" or key == "size" or key == "required" or key == "validator" then
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
		elseif key == "value" or key == "required" then
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
