--[[
  || Renders a string of text to the screen.
  || A function for printing the text is also provided.
  ||
  || "_pos" takes an _ivec2_ with the position of the object.
  || "_text" is the text to render to the object. For this tutorial, it'll be read-only.
--]]
function ExampleObject(_pos, _text)
  local self = {
    obj = Object(_pos, Values.IVEC2_ONE), -- Handle to the base Object.
    text = _text                          -- The text to render.
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
    elseif key == "position" or key == "size" then
      return self.obj[key]
    else
      return nil
    end
  end

  -- Configures the metatable for setting properties in an ExampleObject.
  local __mtNewIndex = function(_, key, value)
    if key == "position" or key == "size" then
      self.obj[key] = ivec2(value)
    end
  end

  -- Set the text in the base Object.
  self.obj.size = ivec2(#self.text, 1)
  self.obj:set(function(data, index, _) data[index] = Character(self.text:sub(index, index), Colours.WHITE, Colours.BLACK) end)
  
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
