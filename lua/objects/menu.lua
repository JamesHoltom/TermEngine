function MenuOption(_title, _callback)
	local self = {
		title = _title,
		callback = _callback,
		hovering = false,
		default_fg = vec3(255.0),
		default_bg = vec3(),
		default_hover_fg = vec3(),
		default_hover_bg = vec3(255.0)
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
	
	local _getColors = function()
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
		getColors   = _getColors
	}
end

function MenuObject()
	local self = {
		obj = Object(vec2(), ivec2(1)),
		options = {},
		option_size = ivec2(0, 1),
		active_option = 1
	}
	
	local _refreshOptions = function()
		self.obj.size = ivec2(self.option_size.x, self.option_size.y * #self.options)
		
		for index, option in ipairs(self.options) do
			option.setHovering(self.active_option == index)
			
			local title = option.getTitle()
			local fg, bg = option.getColors()
			local str_table = { string.byte(title, 1, #title) }
			
			for k = 1, self.option_size.x * self.option_size.y do
				if k <= #str_table then
					self.obj.data[(self.obj.size.x * self.option_size.y * (index - 1)) + k] = Glyph(string.char(str_table[k]), fg, bg)
				else
					self.obj.data[(self.obj.size.x * self.option_size.y * (index - 1)) + k] = empty_glyph
				end
			end
		end
		
		objects.dirty()
	end
	
	local _addOption = function(_index, _object)
		if _index >= 1 and _index <= #self.options + 1 then
			table.insert(self.options, _index, _object)
			
			_refreshOptions()
		else
			print("Cannot add option '" .. _object.getTitle() .. "' to index '" .. _index .. "'.")
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
	
	local _setActive = function(_value)
		self.obj.active = _value
	end
	
	local _doKeyInput = function(key)
		if self.obj.active then
			if key == "Up" then
				self.active_option = ((self.active_option - 2) % #self.options) + 1
				
				_refreshOptions()
			end
			
			if key == "Down" then
				self.active_option = (self.active_option % #self.options) + 1
				
				_refreshOptions()
			end
			
			if key == "Space" then
				self.options[self.active_option].fire()
			end
		end
	end
	
	return {
		doKeyInput = _doKeyInput,
		addOption = _addOption,
		removeOption = _removeOption,
		getActiveOption = _getActiveOption,
		getOptionSize = _getOptionSize,
		setOptionSize = _setOptionSize,
		setActive = _setActive
	}
end
