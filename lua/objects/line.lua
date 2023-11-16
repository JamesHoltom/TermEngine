--[[
-- @author James Holtom
--]]

local empty_character = Character(characters.NO_CHARACTER, characters.DEFAULT_FOREGROUND_COLOUR, characters.DEFAULT_BACKGROUND_COLOUR)

--[[
-- @brief Extends the GameObject usertype to make drawing lines of characters simpler.
-- @param _start      The start position of the line.
-- @param _end        The end position of the line.
-- @param _char       The character to render the line with.
-- @param _game_scene The game scene to add the object to.
--]]
function LineObject(_start, _end, _char, _game_scene)
  local self = {
    object = GameObject(Values.IVEC2_ZERO, Values.IVEC2_ONE, _game_scene or "default"), -- @brief Handle to the Object.
    startPosition = _start,                                                             -- @brief The starting position of the line.
    endPosition = _end,                                                                 -- @brief The ending position of the line.
    character = _char                                                                   -- @brief The Character to use for the line.
  }

  -- @brief Refreshes the object data with the updated settings.
  local _setData = function()
    local pos
    local size = (self.endPosition - self.startPosition):abs() + 1

    if size == Values.IVEC2_ZERO then
      return
    elseif size == Values.IVEC2_ONE then
      self.object.data.set(1, self.character)
      return
    end

    -- Case 1: start is in the top-left, end is in the bottom-right
    if self.startPosition.x <= self.endPosition.x and self.startPosition.y <= self.endPosition.y then
      pos = self.startPosition
    -- Case 2: start is in the top-right, end is in the bottom-left
    elseif self.startPosition.x > self.endPosition.x and self.startPosition.y <= self.endPosition.y then
      pos = Ivec2(self.endPosition.x, self.startPosition.y)
    -- Case 3: start is in the bottom-left, end is in the top-right
    elseif self.startPosition.x <= self.endPosition.x and self.startPosition.y > self.endPosition.y then
      pos = Ivec2(self.startPosition.x, self.endPosition.y)
    -- Case 4: start is in the bottom-right, end is in the top-left
    else
      pos = self.endPosition
    end

    self.object.position = pos
    self.object.data.size = size
    self.object:set(function(_, _) return empty_character end)

    local startPos = self.startPosition - self.object.position
    local endPos = self.endPosition - self.object.position
    local diff = Vec2(self.endPosition - self.startPosition)
    local iterLimit

    -- When the line is taller, iterate on the y-axis to ensure every row has a character.
    if size.x < size.y then
      iterLimit = size.y - 1
    -- When the line is wider, iterate on the x-axis to ensure every column has a character.
    else
      iterLimit = size.x - 1
    end

    for i = 0, iterLimit do
      local iterDiff = Vec2(i/iterLimit) * diff
      local index = getIndexFromRowCol(self.object.data.size, startPos + Ivec2(iterDiff:round()))

      self.object.data.set(index, self.character)
    end
  end

  -- @brief Cleans up the object after use.
  local _release = function(_)
    self.object:release()
  end

  --[[
	-- @brief Configures the metatable for getting properties.
	-- @param key The name of the property to retrieve.
	-- @returns The value of the property.
	--]]
	local mtIndex = function(_, key)
    if key == "id" or key == "position" or key == "active" then
      return self.object[key]
    elseif key == "size" then
      return self.object.data.size
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
    if key == "startPosition" or key == "endPosition" then
      if value.__type.name == "Ivec2" then
        self[key] = value
        
        _setData()
      end
    elseif key == "character" then
      self.character = string.sub(value, 1, 1)

      _setData()
    elseif key == "active" then
      self.object.active = value == true
    end
  end
			
  _setData()

  return setmetatable({
    release     = _release
  }, {
    __index     = mtIndex,
		__newindex  = mtNewIndex,
    __type      = { name = "LineObject" }
  })
end
