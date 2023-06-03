--[[
-- @author James Holtom
--]]

local empty_character = Character(characters.NO_CHARACTER, characters.DEFAULT_FOREGROUND_COLOUR, characters.DEFAULT_BACKGROUND_COLOUR)

--[[
-- @brief Extends the Object usertype to make writing text simpler.
-- @param _position 	The position of the textbox.
-- @param _size 			The size of the textbox.
-- @param _text 			The text to write.
-- @param _game_scene The game scene to add the object to.
--]]
function TextObject(_position, _size, _text, _game_scene)
	local self = {
		obj = GameObject(_position, _size, _game_scene or "default"),			-- @brief Handle to the Object.
		text = tostring(_text or ""),																			-- @brief The text to render.
		fit_text = false,																									-- @brief Should the background colour fit the text, or the Object bounds?
		tab_size = 2,
		fg_colour = characters.DEFAULT_FOREGROUND_COLOUR,									-- @brief The text colour.
		bg_colour = characters.DEFAULT_BACKGROUND_COLOUR									-- @brief The background colour.
	}

	-- @brief Refreshes the object data with the updated settings.
	local _setData = function()
		local text_pos = 1
		local byte_pos = 1
		local tabs_left = 0
		local newline = false

		local setFunc = function(_, index)
			if newline and math.fmod(index, self.obj.size.x) == 1 then
				newline = false
			end
			
			if text_pos <= utf8.len(self.text) and not newline and tabs_left == 0 then
				local code = utf8.codepoint(self.text, byte_pos)
				local chr = utf8.char(code)
				local ret = nil

				if chr == '\n' then
					newline = true
					ret = Character(" ", self.fg_colour, self.bg_colour)
				elseif chr == '\t' then
					tabs_left = self.tab_size - 1
					ret = Character(" ", self.fg_colour, self.bg_colour)
				else
					ret = Character(chr, self.fg_colour, self.bg_colour)
				end

				text_pos = text_pos + 1
				byte_pos = utf8.offset(self.text, text_pos)

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
		end

		self.obj:set(setFunc)
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
		elseif key == "text" or key == "fit_text" or key == "tab_size" or key == "fg_colour" or key == "bg_colour" then
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
		elseif key == "tab_size" then
			self.tab_size = value
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
