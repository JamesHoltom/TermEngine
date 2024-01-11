--[[
-- @author James Holtom
--]]

--[[
-- @brief Extends the GameObject usertype to make labelling fields in a form simpler.
-- @note Clicking on a LabelControl will select the control it is assigned to.
-- @param _name				The name of the control to label.
-- @param _position		The position of the control.
-- @param _size 			The size of the control.
-- @param _form				The form object this control is associated with.
-- @param _label			The text to write.
-- @param _options		Additional options to configure the control with.
--]]
function LabelControl(_name, _position, _size, _form, _label, _options)
	local colours = _options.colours or {}

	local self = {
		name = "",
		object = GameObject(_position, _size , _form.game_scene or defaultGameScene),
		colours = {
			default_text = colours.default_text or characters.DEFAULT_FOREGROUND_COLOUR,
			default_bg = colours.default_bg or characters.DEFAULT_BACKGROUND_COLOUR,
			selected_text = colours.selected_text or characters.DEFAULT_FOREGROUND_COLOUR,
			selected_bg = colours.selected_bg or Colours.DARK_GREY
		},
		label = _label
	}

	local _refreshControl = function()
		local text_colour, bg_colour

		if self.object.hovering == true then
			text_colour = self.colours.selected_text
			bg_colour = self.colours.selected_bg
		else
			text_colour = self.colours.default_text
			bg_colour = self.colours.default_bg
		end

		self.object.characterMap:set(SetText(self.object, self.label, text_colour, bg_colour, false, 2))
	end

	local _setControl = function(_control)
		for k, v in pairs(_form.controls) do
			if v.name == _control then
				self.name = _control

				return
			end
		end

		error('Cannot find control "'.._control..'" to assign label to!')
	end

	local _doMouseEvents = function(_selected, _data)
		if _selected == true then
			_form.selectControl(self.name)
			textInput.start()
		end

		_refreshControl()
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
		elseif key == "label" then
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
				self.object.characterMap.size = value
			end
		elseif key == "name" then
			_setControl(tostring(value))
		end
	end

	_setControl(_name)
	_refreshControl()

	return setmetatable({
		setControl = _setControl,
		doMouseEvents = _doMouseEvents,
		refreshControl = _refreshControl
	}, {
		__index    	= mtIndex,
		__newindex 	= mtNewIndex,
		__type			= { name = "LabelControl" }
	})
end
