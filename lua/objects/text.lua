--[[
-- @author James Holtom
--]]

local empty_character = Character(characters.NO_CHARACTER, characters.DEFAULT_FOREGROUND_COLOUR, characters.DEFAULT_BACKGROUND_COLOUR)

--[[
-- @brief Extends the Object usertype to make writing text simpler.
-- @param _position The position of the textbox.
-- @param _size The size of the textbox.
-- @param _text The text to write.
--]]
function TextObject(_position, _size, _text)
	local self = {
		obj = Object(_position, _size),										-- @brief Handle to the Object.
		text = tostring(_text or ""),											-- @brief The text to render.
		fit_text = false,																	-- @brief Should the background colour fit the text, or the Object bounds?
		fg_colour = characters.DEFAULT_FOREGROUND_COLOUR,	-- @brief The text colour.
		bg_colour = characters.DEFAULT_BACKGROUND_COLOUR	-- @brief The background colour.
	}

	-- @brief Refreshes the object data with the updated settings.
	local _setData = function()
		local text_pos = 1
		local tabs_left = 0
		local newline = false

		self.obj:set(function(data, index)
			if newline and math.fmod(index, self.obj.size.x) == 1 then
				newline = false
			end
			
			if text_pos <= #self.text and not newline and tabs_left == 0 then
				local chr = self.text:sub(text_pos, text_pos)
				local ret = nil

				if chr == '\n' then
					newline = true
					ret = Character(" ", self.fg_colour, self.bg_colour)
				elseif chr == '\t' then
					tabs_left = font.tabSize - 1
					ret = Character(" ", self.fg_colour, self.bg_colour)
				else
					ret = Character(chr, self.fg_colour, self.bg_colour)
				end

				text_pos = text_pos + 1

				return ret
			else
				if self.fit_text then
					return empty_character
				else
					return Character(" ", self.fg_colour, self.bg_colour)
				end

				if tabs_left > 0 then
					tabs_left = tabs_left - 1
					return Character(" ", self.fg_colour, self.bg_colour)
				end
			end
		end)
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
			self.obj.position = ivec2(value)
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
			self[key] = vec4(value)
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
