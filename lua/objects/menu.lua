--[[
-- @author James Holtom
--]]

--[[
-- @brief Extends the GameObject usertype to make menus simpler to setup.
-- @param _position 	The position of the menu.
-- @param _game_scene The game scene to add options to.
--]]
function MenuObject(_position, _game_scene)
	local menu_self = {
		position = _position,										-- @brief The position of the object.
		size = Values.IVEC2_ZERO,								-- @brief The size of the object.
		use_keys = false,												-- @brief Are keyboard controls enabled for the menu?
		selected_option = 1,										-- @brief The index of the option currently selected.
		active = true,													-- @brief Is the menu (and all of its options) active?
		next_y_position = 0,										-- @brief The Y position for the next option.
		options = {},														-- @brief The list of options.
		key_handler = nil,											-- @brief The listener for "key_down" events.
		game_scene = _game_scene or "default"		-- @brief The game scene to add options to.
	}

	--[[
	-- @brief Represents an option in a MenuObject.
	-- @param _title		The title of the option.
	-- @param _position The position of the option.
	-- @param _size 		The size of the option.
	-- @param _callback	The function to call when the option is triggered.
	--]]
	local MenuOption = function(_title, _position, _size, _callback)
		local opt_self = {
			object = GameObject(_position, _size, menu_self.game_scene),	-- @brief Handle to the GameObject.
			title = _title,																								-- @brief The title of the option.
			callback = _callback,																					-- @brief The function to call when the option is triggered.
		}
		
		-- @brief Triggers the option's callback function.
		local _fire = function()
			opt_self.callback()
		end

		-- @brief Refreshes the object data with the updated settings.
		local _setData = function()
			if opt_self.object.active == true then
				local fg_colour = characters.DEFAULT_FOREGROUND_COLOUR
				local bg_colour = characters.DEFAULT_BACKGROUND_COLOUR

				if opt_self.object.id == GetSelectedObject() then
					fg_colour = characters.DEFAULT_BACKGROUND_COLOUR
					bg_colour = characters.DEFAULT_FOREGROUND_COLOUR
				elseif opt_self.object.hovering == true or (menu_self.use_keys == true and opt_self.object.id == menu_self.options[menu_self.selected_option]) then
					bg_colour = Colours.DARK_GREY
				end

				opt_self.object:set(SetText(opt_self.object, tostring(opt_self.title), fg_colour, bg_colour, false, 2))
			end
		end
		
		--[[
		-- @brief Updates the object with data from `object_hover` events.
		-- @param _event The event data.
		--]]
		local _doHoverEvent = function(_event)
			_setData()
		end

		--[[
		-- @brief Updates the object with data from `mouse_down` events.
		-- @param _event The event data.
		--]]
		local _doClickEvent = function(_event)
			if opt_self.object.hovering == true then
				_fire()
			end
			
			_setData()
		end

		-- @brief Cleans up the object after use.
		local _release = function()
			UnsetInteractive(opt_self)
			opt_self.object.active = false
			opt_self.object:release()
		end

		--[[
		-- @brief Configures the metatable for getting properties.
		-- @param key The name of the property to retrieve.
		-- @returns The value of the property.
		--]]
		local mtIndex = function(_, key)
			if key == "id" or key == "position" or key == "hovering" or key == "active" then
				return opt_self.object[key]
			elseif key == "size" then
				return opt_self.object.data.size
			elseif key == "title" then
				return opt_self[key]
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
				opt_self.title = tostring(value)
			elseif key == "hovering" then
				opt_self.hovering = value == true
			elseif key == "active" then
				opt_self.object.active = value == true
			end

			_setData()
		end
		
		_setData()	
	
		return SetInteractive(setmetatable({
			fire 						= _fire,
			doHoverEvent 		= _doHoverEvent,
			doClickEvent 		= _doClickEvent,
			setData 				= _setData,
			release 				= _release
		}, {
			__index 				= mtIndex,
			__newindex 			= mtNewIndex,
			__type					= { name= "MenuOption" }
		}))
	end

	--[[
	-- @brief Adds an option to the menu.
	-- @param _index    The index of the new option.
	-- @param _title    The title of the new option.
	-- @param _callback The function to call when the new option is selected.
	--]]
	local _addOptions = function(...)
		local arg = {...}
		local title
		local position
		local size
		local callback
		local object

		if type(arg[1]) == "string" then
			title = arg[1]
			position = Ivec2(arg[2], menu_self.next_y_position)
			size = arg[3]
			callback = arg[4]

			local newOpt = MenuOption(title, menu_self.position + position, size, callback)

			table.insert(menu_self.options, newOpt.id)
			menu_self.size = menu_self.size:max(position + size)
			menu_self.next_y_position = menu_self.next_y_position + size.y
		elseif type(arg[1]) == "table" then
			for _, v in ipairs(arg) do
				title = v.title or ""
				position = Ivec2(v.offset or 0, menu_self.next_y_position)
				callback = v.callback or nil

				if v.size ~= nil then
					size = Ivec2(v.size)
				else
					size = Ivec2(utf8.len(title), 1)
				end

				local newOpt = MenuOption(title, menu_self.position + position, size, callback)

				table.insert(menu_self.options, newOpt.id)

				menu_self.size = menu_self.size:max(position + size)

				if v.new_line ~= false then
					menu_self.next_y_position = menu_self.next_y_position + size.y
				end
			end
		else
			print("Unknown menu option parameters given!")
		end
	end
	
	--[[
	-- @brief Removes an option from the menu.
	-- @param _index The index of the option to remove.
	--]]
	local _removeOption = function(_index)
		if _index >= 1 and _index <= #menu_self.options then
			table.remove(objects, menu_self.options[_index])
			table.remove(menu_self.options, _index)

			menu_self.selected_option = math.max(menu_self.selected_option, #menu_self.options)
		else
			print("Cannot remove option from index '" .. _index .. "'.")
		end
	end

	-- @brief Refreshes the object data with the updated settings.
	local _refreshOptions = function()
		for k, _ in ipairs(menu_self.options) do
			local object = GetObject(menu_self.options[k])

			if object ~= nil then
				object:setData()
			end
		end
	end
	
	menu_self.key_handler = SetInteractive({
		position = Values.IVEC2_ZERO,
		size = Values.IVEC2_ZERO,
		doKeyEvent = function(_event)
			if menu_self.use_keys ~= true then
				return
			end

			if _event.key == "Up" then
				if menu_self.selected_option == 1 then
					menu_self.selected_option = #menu_self.options
				else
					menu_self.selected_option = menu_self.selected_option - 1
				end
			elseif _event.key == "Down" then
				if menu_self.selected_option == #menu_self.options then
					menu_self.selected_option = 1
				else
					menu_self.selected_option = menu_self.selected_option + 1
				end
			elseif _event.key == "Space" or _event.key == "Enter" then
				SetSelectedObject(menu_self.options[menu_self.selected_option])
				local object = GetObject(menu_self.options[menu_self.selected_option])
				
				if object ~= nil then
					object:fire()
				end
			end

			_refreshOptions()
		end
	})

	-- @brief Cleans up the object after use.
	local _release = function(_)
		UnsetInteractive(menu_self.key_handler)

		for k, _ in ipairs(menu_self.options) do
			menu_self.options[k]:release()
		end
	end

	--[[
	-- @brief Configures the metatable for getting properties.
	-- @param key The name of the property to retrieve.
	-- @returns The value of the property.
	--]]
	local mtIndex = function(_, key)
		if key == "position" or key == "size" or key == "active" or key == "options" or key == "use_keys" then
			return menu_self[key]
		elseif key == "selected_option" then
			return menu_self.options[menu_self.selected_option]
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
				menu_self.position = value
				for k, v in ipairs(menu_self.options) do
					menu_self.options[k].position = v.position + menu_self.position
				end
			end
		elseif key == "active" then
			local flag = (value == true)

			menu_self.active = flag

			for k, _ in ipairs(menu_self.options) do
				menu_self.options[k].active = flag
			end
		elseif key == "use_keys" then
			local flag = (value == true)

			if menu_self.use_keys ~= flag then
				menu_self.selected_option = 1
			end

			menu_self.use_keys = flag
		elseif key == "selected_option" then
			if type(value) == "number" and value >= 1 and value <= #menu_self.options then
				menu_self.selected_option = value
			end
		end

		_refreshOptions()
	end

	return setmetatable({
		addOptions 		= _addOptions,
		removeOption 	= _removeOption,
		doKeyEvent		= _doKeyEvent,
		release 			= _release
	}, {
		__index    		= mtIndex,
		__newindex 		= mtNewIndex,
		__type				= { name = "MenuObject" }
	})
end
