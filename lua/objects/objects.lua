empty_glyph = Glyph(glyphs.NO_CHARACTER, glyphs.DEFAULT_FOREGROUND_COLOUR, glyphs.DEFAULT_BACKGROUND_COLOUR)
space_glyph = Glyph(' ', glyphs.DEFAULT_FOREGROUND_COLOUR, glyphs.DEFAULT_BACKGROUND_COLOUR)

function BoxObject(_position, _size)
	local self = {
		obj = Object(_position, _size),
		fill = empty_glyph,
		outline = empty_glyph,
		has_outline = false
	}
	
	local _setData = function()
		local w = self.obj.size.x
		
		for data_pos = 1, #self.obj.data do
			if self.has_outline and (data_pos <= w or data_pos > (#self.obj.data - w) or (data_pos % w) <= 1) then
				self.obj.data[data_pos] = self.outline
			else
				self.obj.data[data_pos] = self.fill
			end
		end
		
		objects.dirty(true)
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
		fg_colour = glyphs.DEFAULT_FOREGROUND_COLOUR,
		bg_colour = glyphs.DEFAULT_BACKGROUND_COLOUR,
		fit_text = false
	}
	
	local _setData = function()
		local str_table = { string.byte(self.text, 1, #self.text) }
		
		local data_count = self.obj.size.x * self.obj.size.y
		local data_pos = 1
		local text_pos = 1
		local newline = false

		while data_pos <= data_count do
			if newline and math.fmod(data_pos - 1, self.obj.size.x) == 0 then
				newline = false
			end
			
			if text_pos <= #str_table and not newline then
				local chr = string.char(str_table[text_pos])

				if chr == '\n' then
					newline = true
				elseif chr == '\t' then
					data_pos = data_pos + font.tabSize - 1
				else
					self.obj.data[data_pos] = Glyph(chr, self.fg_colour, self.bg_colour)
				end

				text_pos = text_pos + 1
			else
				if self.fit_text then
					self.obj.data[data_pos] = empty_glyph
				else
					self.obj.data[data_pos] = space_glyph
				end
			end

			data_pos = data_pos + 1
		end
		
		objects.dirty(true)
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
	
	local fitText = function(_fit_text)
		self.fit_text = _fit_text

		_setData()
	end
	
	local getColours = function()
		return self.fg_colour, self.bg_colour
	end
	
	local setColours = function(_fg_colour, _bg_colour)
		self.fg_colour = _fg_colour or self.fg_colour
		self.bg_colour = _bg_colour or self.bg_colour

		_setData()
	end

	return {
		getPosition = getPosition,
		setPosition = setPosition,
		getSize = getSize,
		setSize = setSize,
		getText = getText,
		setText = setText,
		fitText = fitText,
		getColours = getColours,
		setColours = setColours
	}
end
