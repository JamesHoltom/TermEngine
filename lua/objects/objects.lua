empty_glyph = Glyph(glyphs.NO_CHARACTER, glyphs.DEFAULT_FG, glyphs.DEFAULT_BG)

function BoxObject(_position, _size)
	local self = {
		obj = Object(_position, _size),
		fill = empty_glyph,
		outline = empty_glyph,
		has_outline = false
	}
	
	local _setData = function()
		local w = self.obj.size.x
		
		for k = 1, #self.obj.data do
			if self.has_outline and (k <= w or k > (#self.obj.data - w) or (k % w) <= 1) then
				self.obj.data[k] = self.outline
			else
				self.obj.data[k] = self.fill
			end
		end
		
		objects.dirty()
	end
	
	local getPosition = function()
		return self.obj.position
	end
	
	local setPosition = function(_position)
		self.obj.position = _position
	end
	
	local getSize = function()
		return self.obj.size
	end
	
	local setSize = function(_size)
		self.obj.size = _size
		
		_setData()
	end
	
	local getFill = function()
		return self.fill
	end
	
	local setFill = function(_glyph)
		self.fill = _glyph
		
		_setData()
	end
	
	local hasOutline = function()
		return self.has_outline
	end
	
	local getOutline = function()
		return self.outline
	end
	
	local setOutline = function(_glyph)
		self.outline = _glyph
		
		if _glyph.character == glyphs.NO_CHARACTER then
			self.has_outline = false
		else
			self.has_outline = true
		end
		
		_setData()
	end
	
	local unsetOutline = function()
		self.outline = empty_glyph
		self.has_outline = false
		
		_setData()
	end
	
	return {
		getPosition = getPosition,
		setPosition = setPosition,
		getSize = getSize,
		setSize = setSize,
		getFill = getFill,
		setFill = setFill,
		hasOutline = hasOutline,
		getOutline = getOutline,
		setOutline = setOutline,
		unsetOutline = unsetOutline
	}
end

function TextObject(_position, _size)
	local self = {
		obj = Object(_position, _size),
		text = "",
		fg_color = glyphs.DEFAULT_FG,
		bg_color = glyphs.DEFAULT_BG
	}
	
	function _setData()
		local str_table = { string.byte(self.text, 1, #self.text) }
		
		for k = 1, #self.obj.data do
			if k <= #str_table then
				self.obj.data[k] = Glyph(string.char(str_table[k]), self.fg_color, self.bg_color)
			else
				self.obj.data[k] = empty_glyph
			end
		end
		
		objects.dirty()
	end
	
	local getPosition = function()
		return self.obj.position
	end
	
	local setPosition = function(_position)
		self.obj.position = _position
	end
	
	local getSize = function()
		return self.obj.position
	end
	
	local setSize = function(_size)
		self.obj.size = _size
		
		_setData()
	end
	
	local getText = function()
		return self.text
	end
	
	local setText = function(_text)
		self.text = tostring(_text)
		
		_setData()
	end
	
	local getColors = function()
		return self.fg_color, self.bg_color
	end
	
	local setColors = function(_fg_color, _bg_color)
		self.fg_color = _fg_color or self.fg_color
		self.bg_color = _bg_color or self.bg_color
		
		_setData()
	end
	
	return {
		getPosition = getPosition,
		setPosition = setPosition,
		getSize = getSize,
		setSize = setSize,
		getText = getText,
		setText = setText,
		getColors = getColors,
		setColors = setColors
	}
end
