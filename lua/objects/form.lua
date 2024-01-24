--[[
-- @author James Holtom
--]]

local is_init = false
local selected_control = false
local selection_mode = "key"
local controls = {}
local events = {}

local function InitFormEvents()
	if is_init == false then
		events = {
			hover = EventListener("object_hover", function(_, _data)
				for k, v in ipairs(controls) do
					if type(v) == "CheckboxControl" then
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
					if _data.rowcol >= v.position and _data.rowcol < v.position + v.size then
						if old_control ~= false and old_control ~= k then
							if type(controls[old_control].doMouseEvents) == "function" then
								controls[old_control].doMouseEvents(_data)
							end

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

				if old_control ~= false and selected_control == false then
					if type(controls[old_control].doMouseEvents) == "function" then
						controls[old_control].doMouseEvents(_data)
					end

					controls[old_control].onSelect(false)
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

function FormObject(_name, _game_scene)
  local self = {
		name = _name,
		game_scene = _game_scene or defaultGameScene,
		controls = {},
		validation_object = nil,
		active = true
  }

	local _setValidationObj = function(_, _position, _size)
		self.validation_object = TextObject(_position, _size, "")
		self.validation_object.fit_text = true
		self.validation_object.active = false
		self.validation_object.fg_colour = characters.DEFAULT_FOREGROUND_COLOUR
		self.validation_object.bg_colour = Colours.RED
	end

	local _removeValidationObj = function()
		self.validation_object:release()
		self.validation_object = nil
	end

	local _raiseValidationError = function(_, _msg)
		self.validation_object.active = true
		self.validation_object.text = tostring(_msg)
	end

	local _clearValidationErrors = function()
		self.validation_object.text = ""
		self.validation_object.active = false

		for k, v in pairs(self.controls) do
			if type(controls[v]) ~= "LabelControl" then
				controls[v].clearValidation()
			end
		end
	end

	local _submit = function()
		local validated, allData, allErrors = true, {}, {}

		for k, v in pairs(self.controls) do
			local control = controls[v]

			if type(control) ~= "LabelControl" then
				local result, data, error = control:validate()
				
				if result == false and validated == true then
					_raiseValidationError(self, error)

					validated = false
				end

				allData[control.name] = data
				allErrors[control.name] = error
			end
		end

		if validated == true then
			_clearValidationErrors()
		end

		return allData, allErrors
	end

	local _addControl = function(s, _args)
		local args = _args or {}
		local ctlType = args.type or ""
		local name = args.name or ""
		local position = args.position or Values.IVEC2_ZERO
		local size = args.size or 1
		local obj = nil

		if ctlType == "text" then
			obj = TextControl(name, position, size, s, {
				value = tostring(args.value or ""),
				max_length = tonumber(args.max_length),
				required = (args.required == true),
				validator = args.validator,
				colours = args.colours or {},
				label = args.label,
				label_size = args.label_size
			})
		elseif ctlType == "number" then
			obj = NumberControl(name, position, size, s, {
				value = tonumber(args.value),
				minimum = args.minimum or false,
				maximum = args.maximum or false,
				enable_step = (args.enable_step == true),
				step_value = args.step_value or 1,
				required = (args.required == true),
				validator = args.validator,
				colours = args.colours or {},
				label = args.label,
				label_size = args.label_size
			})
		-- elseif ctlType == "slider" then
		-- 	obj = SliderControl(name, size, options)
		-- elseif ctlType == "slider2d" then
		-- 	obj = Slider2DControl(name, size, options)
		elseif ctlType == "checkbox" then
			obj = CheckboxControl(name, position, s, {
				options = args.options or {},
				value_texts = args.value_texts or {},
				required = (args.required == true),
				select_multiple = (args.select_multiple == true),
				validator = args.validator,
				colours = args.colours or {},
				label = args.label,
				label_size = args.label_size
			})
		-- elseif ctlType == "dropdown" then
		-- 	obj = DropdownControl(name, size, options)
		end

		if obj ~= nil then
			table.insert(controls, #controls + 1, obj)
			table.insert(self.controls, #self.controls + 1, #controls)
		else
			print("Invalid type given!")
		end
	end

	local _addControls = function(s, ...)
		local arg = {...}

		for k, v in ipairs(arg) do
			if type(v) == "table" then
				_addControl(s, v)
			else
				print("Invalid argument given at index "..k.."!")
			end
		end
	end

	local _removeControl = function(_name)
		for k, v in pairs(self.controls) do
			if controls[v].name == _name then
				controls[v]:release()
				controls[v] = nil
				table.remove(self.controls, k)
			end
		end
	end

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
