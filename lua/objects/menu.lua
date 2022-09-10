function MenuOption(_title, _callback)
	local self = {
		title = _title,
		callback = _callback,
		hovering = false,
		default_fg = Colours.WHITE,
		default_bg = Colours.BLACK,
		default_hover_fg = Colours.BLACK,
		default_hover_bg = Colours.WHITE
	}
	
	local _getTitle = function()
		return self.title
	end
	
	local _setTitle = function(_title)
		self.title = _title
	end
	
	local _fire = function()
		self.callback()
	end
	
	local _isHovering = function()
		return self.hovering
	end
	
	local _setHovering = function(_value)
		self.hovering = _value
	end
	
	local _getColours = function()
		if self.hovering then
			return self.default_hover_fg, self.default_hover_bg
		else
			return self.default_fg, self.default_bg
		end
	end
	
	return {
		getTitle    = _getTitle,
		setTitle    = _setTitle,
		fire        = _fire,
		isHovering  = _isHovering,
		setHovering = _setHovering,
		getColours  = _getColours
	}
end

function MenuObject(_position)
	local self = {
		obj = Object(_position, ivec2(1)),
		events = {},
		options = {},
		option_size = ivec2(0, 1),
		active_option = 1
	}

	local _refreshOptions = function()
		self.obj.size = ivec2(self.option_size.x, self.option_size.y * #self.options)
		
		for index, option in ipairs(self.options) do
			option.setHovering(self.active_option == index)
			
			local title = option.getTitle()
			local fg, bg = option.getColours()
			local str_table = { string.byte(title, 1, #title) }
			
			for k = 1, self.option_size.x * self.option_size.y do
				if k <= #str_table then
					self.obj.data[(self.obj.size.x * self.option_size.y * (index - 1)) + k] = Glyph(string.char(str_table[k]), fg, bg)
				else
					self.obj.data[(self.obj.size.x * self.option_size.y * (index - 1)) + k] = empty_glyph
				end
			end
		end
		
		objects.dirty(true)
	end
	
	local _addOption = function(_index, _params)
		if _index >= 1 and _index <= #self.options + 1 then
			table.insert(self.options, _index, MenuOption(_params.title, _params.callback))

			self.option_size.x = math.max(#_params.title, self.option_size.x)

			_refreshOptions()
		else
			print("Cannot add option '" .. _params.title .. "' to index '" .. _index .. "'.")
		end
	end
	
	local _removeOption = function(_index)
		if _index >= 1 and _index <= #self.options then
			table.remove(self.options, _index)
			
			_refreshOptions()
		else
			print("Cannot remove option from index '" .. _index .. "'.")
		end
	end
	
	local _getActiveOption = function()
		return self.active_option
	end
	
	local _getOptionSize = function()
		return self.option_size
	end
	
	local _setOptionSize = function(_size)
		self.option_size = _size
		
		_refreshOptions()
	end

	local _doKeyInput = function(_key)
			if _key == "Up" then
				if self.active_option == 1 then
					self.active_option = #self.options
				else
					self.active_option = self.active_option - 1
				end
				
				_refreshOptions()
			end
			
			if _key == "Down" then
				if self.active_option == #self.options then
					self.active_option = 1
				else
					self.active_option = self.active_option + 1
				end
				
				_refreshOptions()
			end
			
			if _key == "Space" then
				self.options[self.active_option].fire()
			end
	end

	local _isActive = function()
		return self.obj.active
	end

	local _setActive = function(_flag)
		self.obj.active = _flag
		self.events.keydown.active = _flag
	end

	local _release = function(_)
		self.obj:release()

		for k, v in pairs(self.events) do
			v:release()
		end
	end
	
	self.events.keydown = EventListener("key_down", function(this, event)
		_doKeyInput(event.key)
	end)

	return {
		addOption = _addOption,
		removeOption = _removeOption,
		getActiveOption = _getActiveOption,
		getOptionSize = _getOptionSize,
		setOptionSize = _setOptionSize,
		isActive = _isActive,
		setActive = _setActive,
		release = _release
	}
end
