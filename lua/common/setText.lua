--[[
-- @author James Holtom
--]]

--[[
-- @brief Sets the character data in an object with the given text.
-- @param _text The text to set.
-- @param _fg_colour The foreground colour to set.
-- @param _bg_colour The background colour to set.
-- @param _fit_text Does the background colour fill the object bounds fully, or up to the text?
-- @param _tab_size The number of spaces that make up a tab (i.e. *\t*) character.
--]]
function SetText(obj, _text, _fg_colour, _bg_colour, _fit_text, _tab_size)
  local text_pos = 1
  local byte_pos = 1
  local text_count = utf8.len(_text)
  local tabs_left = 0
  local newline = false

  return function(_, index)
    -- If we previously hit a newline character, and we're now on a new line, start rendering characters.
    if newline == true and math.fmod(index, obj.size.x) == 1 then
      newline = false
    end
    
    --[[
      Render the next character, given the criteria:
        - There is text to render.
        - We haven't hit a newline character, and have to render spaces until the next line is reached.
        - We haven't hit a tab character, and have spaces left to render.
    --]]
    if text_pos <= text_count and not newline and tabs_left == 0 then
      local code = utf8.codepoint(_text, byte_pos)
      local chr = utf8.char(code)
      local ret = nil

      -- If we've hit a newline character, start rendering spaces until we reach the next line.
      if chr == '\n' then
        newline = true
        ret = Character(" ", _fg_colour, _bg_colour)
      -- If we've hit a tab character, start rendering spaces until the tab size is reached.
      elseif chr == '\t' then
        tabs_left = _tab_size - 1
        ret = Character(" ", _fg_colour, _bg_colour)
      else
        ret = Character(chr, _fg_colour, _bg_colour)
      end

      text_pos = text_pos + 1
      byte_pos = utf8.offset(_text, text_pos)

      return ret
    else
      -- If the background colour is set to fit to the text, stop rendering characters.
      if _fit_text then
        return Character(characters.NO_CHARACTER, characters.DEFAULT_FOREGROUND_COLOUR, characters.DEFAULT_BACKGROUND_COLOUR)
      else
        return Character(" ", _fg_colour, _bg_colour)
      end

      -- If we previously hit a tab character, render spaces until the tab size is reached.
      if tabs_left > 0 then
        tabs_left = tabs_left - 1
        return Character(" ", _fg_colour, _bg_colour)
      end
    end
  end
end
