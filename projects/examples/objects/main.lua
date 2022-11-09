local anObj

function Init()
  -- Objects take a position and a size.
  anObj = Object(Values.IVEC2_ZERO, ivec2(3, 2))

  -- All characters in an Object can be modified with the `.set` method.
  anObj:set(function(data, index, _) data[index] = Character("?", Colours.WHITE, Colours.RED) end)

  -- The position and size of the Object can be set by their properties.
  anObj.position = ivec2(2, 3)
  anObj.size = Values.IVEC2_ONE

  -- You can set individual characters in an Object with the `.data` property.
  anObj.data[1] = Character("!", Colours.GREEN, Colours.DARK_GREY)

  return true
end

function Quit()
  -- When an Object is no longer needed, it can be removed with the `.release` method.
  anObj:release()

  return true
end