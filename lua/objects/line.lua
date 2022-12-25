--[[
-- @author James Holtom
--]]

local empty_character = Character(characters.NO_CHARACTER, characters.DEFAULT_FOREGROUND_COLOUR, characters.DEFAULT_BACKGROUND_COLOUR)

--[[
-- @brief Extends the Object usertype to make drawing lines of characters simpler.
-- @param _start The start position of the line.
-- @param _end The end position of the line.
-- @param _char The character to render the line with.
--]]
function LineObject(_start, _end, _char)
  local self = {
    obj = Object(Values.IVEC2_ZERO, Values.IVEC2_ONE),
    startPosition = _start,
    endPosition = _end,
    character = _char
  }

  -- @brief Refreshes the object data with the updated settings.
  local _setData = function()
    local pos
    local size = (self.endPosition - self.startPosition):abs() + 1

    if size == Values.IVEC2_ZERO then
      return
    elseif size == Values.IVEC2_ONE then
      self.obj.data[1] = self.character
      return
    end

    -- Case 1: start is in the top-left, end is in the bottom-right
    if self.startPosition.x <= self.endPosition.x and self.startPosition.y <= self.endPosition.y then
      pos = self.startPosition
    -- Case 2: start is in the top-right, end is in the bottom-left
    elseif self.startPosition.x > self.endPosition.x and self.startPosition.y <= self.endPosition.y then
      pos = ivec2(self.endPosition.x, self.startPosition.y)
    -- Case 3: start is in the bottom-left, end is in the top-right
    elseif self.startPosition.x <= self.endPosition.x and self.startPosition.y > self.endPosition.y then
      pos = ivec2(self.startPosition.x, self.endPosition.y)
    -- Case 4: start is in the bottom-right, end is in the top-left
    else
      pos = self.endPosition
    end

    self.obj.position = pos
    self.obj.size = size
    self.obj:set(function(_, _) return empty_character end)

    local startPos = self.startPosition - self.obj.position
    local endPos = self.endPosition - self.obj.position
    local diff = vec2(self.endPosition - self.startPosition)
    local iterLimit

    -- When the line is taller, iterate on the y-axis to ensure every row has a character.
    if size.x < size.y then
      iterLimit = size.y - 1
    -- When the line is wider, iterate on the x-axis to ensure every column has a character.
    else
      iterLimit = size.x - 1
    end

    for i = 0, iterLimit do
      local iterDiff = vec2(i/iterLimit) * diff
      local index = getIndexFromRowCol(self.obj.size, startPos + ivec2(iterDiff:round()))

      self.obj.data[index] = self.character
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
    if key == "position" or key == "size" then
      return self.obj[key]
    elseif key == "startPosition" or key == "endPosition" or key == "character" then
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
    if key == "startPosition" or key == "endPosition" or key == "character" then
      self[key] = value
			
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
