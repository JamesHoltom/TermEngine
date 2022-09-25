--[[
-- @author James Holtom
--]]

empty_glyph = Glyph(glyphs.NO_CHARACTER, glyphs.DEFAULT_FOREGROUND_COLOUR, glyphs.DEFAULT_BACKGROUND_COLOUR)
space_glyph = Glyph(' ', glyphs.DEFAULT_FOREGROUND_COLOUR, glyphs.DEFAULT_BACKGROUND_COLOUR)

--[[
-- @brief Extends the Object usertype to make drawing boxes simpler.
-- @param _position The position of the box.
-- @param _size The size of the box.
--]]
function BoxObject(_position, _size)
	local self = {
		obj = Object(_position, _size),	-- @brief
		fill = empty_glyph,							-- @brief
		outline = empty_glyph,					-- @brief
		has_outline = false							-- @brief
	}
	
	-- @brief Refreshes the object data with the updated settings.
	local _setData = function()
		local w = self.obj.size.x
		
		for data_pos = 1, #self.obj.data do
			if self.has_outline and (data_pos <= w or data_pos > (#self.obj.data - w) or (data_pos % w) <= 1) then
				self.obj.data[data_pos] = self.outline
			else
				self.obj.data[data_pos] = self.fill
			end
		end
		
		objects.dirty()
	end
	
	--[[
	-- @brief Does the box have an outline?
	-- @returns If the box have an outline.
	--]]
	local _hasOutline = function()
		return self.has_outline
	end
	
	-- @brief Removes the outline from the box.
	local _unsetOutline = function()
		self.outline = empty_glyph
		self.has_outline = false
		
		_setData()
	end

	-- @brief Cleans up the object after use.
	local _release = function(_)
		self.obj:release()
	end

	--[[
	-- @brief Configures the metatable for getting properties.
	-- @param key The name of the property to retrieve.
	-- @returns The value of the property.
	--]]
	local mtIndex = function(o, key)
		if key == "position" or key == "size" or key == "active" then
			return self.obj[key]
		elseif key == "fill" or key == "outline" then
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
	local mtNewIndex = function(o, key, value)
		if key == "position" then
			self.obj.position = vec2(value)
		elseif key == "size" then
			self.obj.size = ivec2(value)
			_setData()
		elseif key == "active" then
			self.obj.active = value
		elseif key == "fill" then
			self[key] = Glyph(value)
			
			_setData()
		elseif key == "outline" then
			self[key] = Glyph(value)

			if value.character == glyphs.NO_CHARACTER then
				self.has_outline = false
			else
				self.has_outline = true
			end
			
			_setData()
		end
	end
	
	return setmetatable({
		hasOutline   = _hasOutline,
		unsetOutline = _unsetOutline,
		release      = _release
	}, {
		__index    = mtIndex,
		__newindex = mtNewIndex
	})
end

--[[
-- @brief Extends the Object usertype to make writing text simpler.
-- @param _position The position of the textbox.
-- @param _size The size of the textbox.
-- @param _text The text to write.
--]]
function TextObject(_position, _size, _text)
	local self = {
		obj = Object(_position, _size),
		text = tostring(_text or ""),
		fit_text = false,
		fg_colour = glyphs.DEFAULT_FOREGROUND_COLOUR,
		bg_colour = glyphs.DEFAULT_BACKGROUND_COLOUR
	}

	-- @brief Refreshes the object data with the updated settings.
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
		
		objects.dirty()
	end
	
	-- @brief Cleans up the object after use.
	local _release = function(_)
		self.obj:release()
	end

	--[[
	-- @brief Configures the metatable for getting properties.
	-- @param key The name of the property to retrieve.
	-- @returns The value of the property.
	--]]
	local mtIndex = function(_, key)
		if key == "position" or key == "size" or key == "active" then
			return self.obj[key]
		elseif key == "text" or key == "fit_text" or key == "fg_colour" or key == "bg_colour" then
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
			self.obj.position = vec2(value)
		elseif key == "size" then
			self.obj.size = ivec2(value)
			_setData()
		elseif key == "active" then
			self.obj.active = value
		elseif key == "text" then
			self.text = tostring(value)
			_setData()
		elseif key == "fit_text" then
			self.fit_text = value
			_setData()
		elseif key == "fg_colour" or key == "bg_colour" then
			self[key] = vec3(value)
			_setData()
		end
	end
	
	_setData()

	return setmetatable({
		release = _release
	}, {
		__index    = mtIndex,
		__newindex = mtNewIndex
	})
end
