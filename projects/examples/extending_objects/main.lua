--[[
  || Renders a string of text to the screen.
  || A function for printing the text is also provided.
  ||
  || "_pos" takes an _ivec2_ with the position of the object.
  || "_text" is the text to render to the object. For this tutorial, it'll be read-only.
  || "_game_scene" is the game scene to render this object to.
--]]
function ExampleObject(_pos, _text, _game_scene)
  local self = {
    obj = GameObject(_pos, Values.IVEC2_ONE, _game_scene or "default"), -- Handle to the base Object.
    text = _text                                                        -- The text to render.
  }

  -- Prints the text property to the console.
  local _print = function(_)
    print(self.text)
  end

  -- Releases the base Object.
  local _release = function(_)
    self.obj:release()
  end

  -- Configures the metatable for retrieving properties from an ExampleObject.
  local __mtIndex = function(_, key)
    if key == "text" then
      return self.text
    elseif key == "position" then
      return self.obj.position
    elseif key == "size" then
      return self.obj.characterMap.size
    else
      return nil
    end
  end

  -- Configures the metatable for setting properties in an ExampleObject.
  local __mtNewIndex = function(_, key, value)
    if key == "position" then
      if value.__type.name == "Ivec2" then
        self.obj.position = Ivec2(value)
      end
    elseif key == "size" then
      if value.__type.name == "Ivec2" and value >= Values.IVEC2_ONE then
        self.obj.characterMap.size = Ivec2(value)
      end
    end
  end

  -- Set the text in the base Object.
  self.obj.characterMap.size = Ivec2(#self.text, 1)
  self.obj.characterMap:set(function(_, index) return Character(self.text:sub(index, index), Colours.WHITE, Colours.BLACK) end)
  
  -- Return the object with the methods and metatable set.
  return setmetatable({
    print       = _print,
    release     = _release
  }, {
    __index     = __mtIndex,
    __newIndex  = __mtNewIndex
  })
end

function Init()
  local exampleObj = ExampleObject(Values.IVEC2_ONE, "Hello!")

  exampleObj:print()

  return true
end
