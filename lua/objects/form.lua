--[[
-- @author James Holtom
--]]

local is_init = false						-- @brief Have the event listeners been initialised?
local selected_control = false	-- @brief The currently selected form control.
local controls = {}							-- @brief The list of form controls.
local events = {}								-- @brief The list of event listeners.

-- @brief Initialises the event listeners for handling form input/selection.
local function InitFormEvents()
	if is_init == false then
		events = {
			hover = EventListener("object_hover", function(_, _data)
				for k, v in ipairs(controls) do
					if type(v) == "CheckboxControl" or type(v) == "DropdownControl" then
						if v.label_id == _data.id then
							controls[k].onHover(_data.type == "over")
							return
						end

						for _, id in ipairs(v.ids) do
							if id == _data.id then
								controls[k].onHover(_data.type == "over")
								return
							end
						end
					elseif v.id == _data.id then
						controls[k].onHover(_data.type == "over")
						return
					end
				end
			end),
			click = EventListener("mouse_down", function(_, _data)
				local old_control = selected_control
				selected_control = false

				for k, v in ipairs(controls) do
					if v.active == true then
						if v.isOver(_data.rowcol) == true then
							if old_control ~= false and old_control ~= k then
								controls[old_control].onSelect(false)
							end
							
							selected_control = k
							controls[selected_control].onSelect(true)

							if type(controls[selected_control].doMouseEvents) == "function" then
								controls[selected_control].doMouseEvents(_data)
							end

							break
						end
					end
				end

				if old_control ~= false and selected_control == false then
					controls[old_control].onSelect(false)
				end
			end),
			move = EventListener("mouse_move", function(_, _data)
				if selected_control ~= false and type(controls[selected_control].doMoveEvents) == "function" then
					controls[selected_control].doMoveEvents(_data)
				end
			end),
			scroll = EventListener("mouse_scroll", function(_, _data)
				if selected_control ~= false and type(controls[selected_control].doScrollEvents) == "function" then
					controls[selected_control].doScrollEvents(_data)
				end
			end),
			keydown = EventListener("key_down", function(_, _data)
				if selected_control ~= false and type(controls[selected_control].doKeyEvents) == "function" then
					controls[selected_control].doKeyEvents(_data)
				end
			end),
			input = EventListener("text_input", function(_, _data)
				if selected_control ~= false and type(controls[selected_control].doTextEvents) == "function" then
					controls[selected_control].doTextEvents(_data)
				end
			end)
		}
		is_init = true
	end
end

--[[
-- @brief Manages a list of form controls, and the validation of user-submitted input.
-- @param _game_scene The game scene this form is rendered to.
--]]
function FormObject(_game_scene)
  local self = {
		game_scene = _game_scene or defaultGameScene,
		controls = {},
		validation_object = nil,
		active = true
  }

	--[[
	-- @brief Sets the position/size of the validation textbox. It is created, if not set.
	-- @param _position The position of the textbox.
	-- @param _size The size of the textbox.
	--]]
	local _setValidationObj = function(_, _position, _size)
		if self.validation_object == nil then
			self.validation_object = TextObject(_position, _size, "")
		end

		self.validation_object.fit_text = true
		self.validation_object.active = false
		self.validation_object.fg_colour = characters.DEFAULT_FOREGROUND_COLOUR
		self.validation_object.bg_colour = Colours.RED
	end

	-- @brief Removes the validation textbox.
	local _removeValidationObj = function()
		if self.validation_object ~= nil then
			self.validation_object:release()
			self.validation_object = nil
		end
	end

	--[[
	-- @brief Prints a custom message to the validation textbox.
	-- @param _msg The message to print.
	--]]
	local _raiseValidationError = function(_msg)
		if self.validation_object ~= nil then
			self.validation_object.active = true
			self.validation_object.text = tostring(_msg)
		end
	end

	-- @brief Clears the validation textbox of text.
	local _clearValidationErrors = function()
		self.validation_object.text = ""
		self.validation_object.active = false

		for k, v in pairs(self.controls) do
			if type(controls[v]) ~= "LabelControl" then
				controls[v].clearValidation()
			end
		end
	end

	--[[
	-- @brief Runs validation on all of the form controls and returns the results.
	-- @returns Table containing the user-submitted data and any validation errors.
	--]]
	local _submit = function()
		local validated, allData, allErrors = true, {}, {}

		for k, v in pairs(self.controls) do
			local control = controls[v]
			local result, data, error = control:validate()
			
			if result == false and validated == true then
				_raiseValidationError(error)

				validated = false
			end

			allData[control.name] = data
			allErrors[control.name] = error
		end

		if validated == true then
			_clearValidationErrors()
		end

		return allData, allErrors
	end

	-- @brief Adds a form control to the object with the given parameters.
	local _addControls = function(s, ...)
		local args = {...}

		for k, v in ipairs(args) do
			if type(v) == "table" then
				local ctlType = v.type or ""
				local name = v.name or ""
				local position = v.position or Values.IVEC2_ZERO
				local size = v.size or 1
				local obj = nil
		
				if ctlType == "text" then
					print(tostring(s.game_scene))
					obj = TextControl(name, position, size, s, {
						value = tostring(v.value or ""),
						max_length = tonumber(v.max_length),
						required = (v.required == true),
						validator = v.validator,
						colours = v.colours or {},
						label = v.label,
						label_size = v.label_size
					})
				elseif ctlType == "number" then
					obj = NumberControl(name, position, size, s, {
						value = tonumber(v.value),
						minimum = v.minimum or false,
						maximum = v.maximum or false,
						enable_step = (v.enable_step == true),
						step_value = v.step_value or 1,
						required = (v.required == true),
						validator = v.validator,
						colours = v.colours or {},
						label = v.label,
						label_size = v.label_size
					})
				elseif ctlType == "checkbox" then
					obj = CheckboxControl(name, position, size, s, {
						options = v.options or {},
						value_texts = v.value_texts or {},
						required = (v.required == true),
						select_multiple = (v.select_multiple == true),
						validator = v.validator,
						colours = v.colours or {},
						label = v.label,
						label_size = v.label_size
					})
				elseif ctlType == "dropdown" then
					obj = DropdownControl(name, position, size, s, {
						options = v.options or {},
						required = (v.required == true),
						select_multiple = (v.select_multiple == true),
						list_count = v.list_count,
						validator = v.validator,
						colours = v.colours or {},
						label = v.label,
						label_size = v.label_size
					})
				end
		
				if obj ~= nil then
					table.insert(controls, #controls + 1, obj)
					table.insert(self.controls, #self.controls + 1, #controls)
				else
					print("Invalid type given!")
				end
			else
				print("Invalid argument given at index "..k.."!")
			end
		end
	end

	--[[
	-- @brief Removes the form control with the given name.
	-- @param _name The name of the control to remove.
	--]]
	local _removeControl = function(_name)
		for k, v in pairs(self.controls) do
			if controls[v].name == _name then
				controls[v]:release()
				controls[v] = nil
				table.remove(self.controls, k)
			end
		end
	end

	--[[
	-- @brief Selects the form control with the given name.
	-- @param _name The name of the control to select.
	--]]
	local _selectControl = function(_name)
		if selected_control ~= false then
			controls[selected_control].onSelect(false)
		end

		for k, v in pairs(self.controls) do
			if controls[v].name == _name then
				controls[v].onSelect(true)
				selected_control = v

				return
			end
		end
	end

	-- @brief De-selects the selected control.
	local _deselectControls = function()
		if selected_control ~= false then
			controls[selected_control].onSelect(false)
			selected_control = false
		end
	end

	-- @brief Cleans up the object after use.
	local _release = function(_)
		for k, v in ipairs(self.controls) do
			self.controls[k]:release()
			table.remove(self.controls, k)
			table.remove(controls, k)
		end

		self.validation_object:release()
	end

	--[[
	-- @brief Configures the metatable for getting properties.
	-- @param key The name of the property to retrieve.
	-- @returns The value of the property.
	--]]
	local mtIndex = function(_, key)
		if key == "active" or key == "game_scene" then
			return self[key]
		elseif key == "selected_control" then
			return selected_control
		elseif key == "controls" then
			local retCtls = {}

			for k, v in pairs(self.controls) do
				table.insert(retCtls, k, controls[v])
			end

			return retCtls
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
		if key == "active" then
			for _, v in pairs(self.controls) do
				controls[v].active = (value == true)
			end
		end
	end

	InitFormEvents()

	return setmetatable({
		setValidationMessage = _setValidationObj,
		removeValidationMessage = _removeValidationObj,
		raiseError = _raiseValidationError,
		clearErrors = _clearValidationErrors,
		submit = _submit,
		addControls = _addControls,
		removeControl = _removeControl,
		selectControl = _selectControl,
		deselectControls = _deselectControls,
		release = _release
	}, {
		__index    = mtIndex,
		__newindex = mtNewIndex
	})
end
