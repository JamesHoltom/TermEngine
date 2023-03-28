local anObj, anotherObj

function Init()
  -- Objects take a position and a size.
  anObj = GameObject(Values.IVEC2_ZERO, ivec2(3, 2))
  anotherObj = GameObject(Values.IVEC2_ZERO, Values.IVEC2_ONE)

  -- All characters in an Object can be modified with the `.set` method.
  anObj:set(function(_, _) return Character("?", Colours.WHITE, Colours.RED) end)

  -- The position and size of the Object can be set by their properties.
  anotherObj.position = ivec2(2, 3)
  anotherObj.size = Values.IVEC2_ONE

  -- You can set individual characters in an Object with the `.data` property.
  anotherObj.data[1] = Character("!", Colours.GREEN, Colours.DARK_GREY)

  return true
end
