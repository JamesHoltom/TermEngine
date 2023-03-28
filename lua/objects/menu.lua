--[[
-- @author James Holtom
--]]

--[[
-- @brief Extends the Object usertype to make menus simpler to setup.
-- @param _position 	The position of the menu.
-- @param _game_scene The game scene to add the object to.
--]]
function MenuObject(_position, _game_scene)
	--[[
	-- @brief Represents an option in a MenuObject.
	-- @param _title		The title of the option.
	-- @param _callback	The function to call when the option is triggered.
	--]]
	local MenuOption = function(_title, _callback)
		local self = {
			title = _title,										-- @brief The title of the option.
			callback = _callback,							-- @brief The function to call when the option is triggered.
			hovering = false,									-- @brief Is this option being selected/hovered over?
			fg_colour = Colours.WHITE,				-- @brief The text colour for the option.
			bg_colour = Colours.BLACK,				-- @brief The background colour for the option.
			fg_colour_hover = Colours.BLACK,	-- @brief The text colour when hovering over the option.
			bg_colour_hover = Colours.WHITE		-- @brief The background colour when hovering over the option.
		}
		
		-- @brief Triggers the option's callback function.
		local _fire = function()
			self.callback()
		end
		
		--[[
		-- @brief Configures the metatable for getting properties.
		-- @param key The name of the property to retrieve.
		-- @returns The value of the property.
		--]]
		local mtIndex = function(_, key)
			if key == "title" or key == "hovering" or key == "fg_colour" or key == "bg_colour" or key == "fg_colour_hover" or key == "bg_colour_hover" then
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
			if key == "title" then
				self.title = tostring(value)
			elseif key == "hovering" then
				self.hovering = value
			elseif key == "fg_colour" or key == "bg_colour" or key == "fg_colour_hover" or key == "bg_colour_hover" then
				self[key] = vec4(value)
			end
		end
	
		return setmetatable({
			fire = _fire
		}, {
			__index = mtIndex,
			__newindex = mtNewIndex
		})
	end

	local self = {
		obj = GameObject(_position, Values.IVEC2_ONE, _game_scene or "default"),	-- @brief Handle to the Object.
		events = {},																															-- @brief The list of event listeners.
		options = {},																															-- @brief The list of options.
		option_width = 0,																													-- @brief The maximum width of the options.
		active_index = 1																													-- @brief The index of the option currently selected.
	}

	-- @brief Refreshes the object data with the updated list of options.
	local _refreshOptions = function()
		local columns = #self.options
		self.obj:size(ivec2(self.option_width, columns))
		
		for index, option in ipairs(self.options) do
			option.hovering = self.active_index == index
			
			local title = option.title
			local fg = option.fg_colour
			local bg = option.bg_colour
			local c_index = 1

			if option.hovering then
				fg = option.fg_colour_hover
				bg = option.bg_colour_hover
			end
			
			for c in title:gmatch(".") do
				self.obj:data()[(self.option_width * (index - 1)) + c_index] = Character(c, fg, bg)
				c_index = c_index + 1
			end

			for i = c_index, self.option_width do
				self.obj:data()[(self.option_width * (index - 1)) + i] = Character(" ", fg, bg)
			end
		end
	end
	
	--[[
	-- @brief Adds an option to the menu.
	-- @param _index    The index of the new option.
	-- @param _title    The title of the new option.
	-- @param _callback The function to call when the new option is selected.
	--]]
	local _addOption = function(_index, _title, _callback)
		if _index >= 1 and _index <= #self.options + 1 then
			table.insert(self.options, _index, MenuOption(_title, _callback))

			self.option_width = math.max(#_title, self.option_width)

			_refreshOptions()
		else
			print("Cannot add option '" .. _title .. "' to index '" .. _index .. "'.")
		end
	end
	
	--[[
	-- @brief Removes an option from the menu.
	-- @param _index The index of the option to remove.
	--]]
	local _removeOption = function(_index)
		if _index >= 1 and _index <= #self.options then
			table.remove(self.options, _index)
			
			_refreshOptions()
		else
			print("Cannot remove option from index '" .. _index .. "'.")
		end
	end
	
	--[[
	-- @brief Event callback for handling menu navigation with the keyboard.
	-- @param _key The key currently being pressed.
	--]]
	local _doKeyInput = function(_key)
		if _key == "Up" then
			if self.active_index == 1 then
				self.active_index = #self.options
			else
				self.active_index = self.active_index - 1
			end
			
			_refreshOptions()
		end
		
		if _key == "Down" then
			if self.active_index == #self.options then
				self.active_index = 1
			else
				self.active_index = self.active_index + 1
			end
			
			_refreshOptions()
		end
		
		if _key == "Space" then
			self.options[self.active_index].fire()
		end
	end

	-- @brief Cleans up the object and event listeners after use.
	local _release = function(_)
		self.obj:release()

		for _, v in pairs(self.events) do
			v:release()
		end
	end

	--[[
	-- @brief Configures the metatable for getting properties.
	-- @param key The name of the property to retrieve.
	-- @returns The value of the property.
	--]]
	local mtIndex = function(_, key)
		if key == "position" or key == "size" or key == "active" then
			return self.obj[key]
		elseif key == "option_width" or key == "options" or key == "active_index" then
			return self[key]
		elseif key == "active_option" then
			return self.options[self.active_index]
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
			self.obj.position = ivec2(value)
		elseif key == "active" then
			self.obj.active = value
			self.events.keydown.active = _flag
		elseif key == "option_width" then
			self.option_width = tonumber(value)
			_refreshOptions()
		elseif key == "active_index" then
			self.active_index = tonumber(value)
		end
	end

	self.events.keydown = EventListener(defaultScene(), "key_down", function(_, event)
		_doKeyInput(event.key)
	end)

	return setmetatable({
		addOption = _addOption,
		removeOption = _removeOption,
		release = _release
	}, {
		__index    = mtIndex,
		__newindex = mtNewIndex
	})
end
