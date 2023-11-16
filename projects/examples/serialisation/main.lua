local vTbl, jTbl

function printCharacter(k, v, depth)
  depth = depth or 1

  local tab = string.rep(" ", depth * 2)

  print(tab .. k .. ": Character - Char(" .. v.character .. "), Foreground(" .. tostring(v.foregroundColour) .. "), Background(" .. tostring(v.backgroundColour) .. ")")
end

function printCharacterMap(k, v, depth)
  depth = depth or 1

  local tab = string.rep(" ", depth * 2)

  print(tab .. k .. ": CharacterMap - Size(" .. tostring(v.size) .. "), Data([")

  for i = 1, #v.data do
    printCharacter(i, v.data[i], depth + 1)
  end

  print(tab .. "])")
end

function printGameObject(k, v, depth)
  depth = depth or 1

  local tab = string.rep(" ", depth * 2)

  print(tab .. k .. ": GameObject - Pos(" .. tostring(v.position) .. "), Size(" .. tostring(v.size) .. "), Active(" .. tostring(v.active) .. "), Data({")

  printCharacterMap("data", v.data, depth + 1)

  print(tab .. "})")
end

function printAnimationFrame(k, v, depth)
  depth = depth or 1

  local tab = string.rep(" ", depth * 2)

  print(tab .. k .. ": AnimationFrame - Size(" .. tostring(v.data.size) .. "), Offset(" .. tostring(v.offset) .. "), Added Duration(" .. v.addedDuration .. "), Data([")

  printCharacterMap("data", v.data, depth + 1)

  print(tab .. "])")
end

function printAnimation(k, v, depth)
  depth = depth or 1

  local tab = string.rep(" ", depth * 2)

  print(tab .. k .. ": Animation - Name(" .. v.name .. "), Frames([")

  for i = 1, #v.frames do
    printAnimationFrame(i, v.frames[i], depth + 1)
  end

  print(tab .. "])")
end

function printTable(table, depth)
  depth = depth or 1

  local tab = string.rep(" ", depth * 2)

  for k,v in pairs(table) do
    if type(v) == "table" then
      print(tab .. k .. ": {")
      printTable(v, depth + 1)
      print(tab .. "}")
    elseif type(v) == "userdata" then
      if v.__type.name == "GameObject" then
        printGameObject(k, v, depth)
      elseif v.__type.name == "Character" then
        printCharacter(k, v, depth)
      elseif v.__type.name == "Animation" then
        printAnimation(k, v, depth)
      elseif v.__type.name == "AnimationFrame" then
        printAnimationFrame(k, v, depth)
      else
        print(tab .. k .. ": " .. v.__type.name .. " - " .. tostring(v))
      end
    else
      print(tab .. k .. ": " .. tostring(v))
    end
  end
end

function Init()
  -- Set up the data for the vON (de)serialisation.
  local vCharacter = Character("v", Colours.RED, Colours.WHITE)
  local wCharacter = Character("w", Colours.WHITE, Colours.RED)
  local vObj = GameObject(Values.IVEC2_ONE, Ivec2(1, 2))
  vObj.data.data[1] = vCharacter
  local vAnim = Animation("von_anim")
  vAnim:addFrame(AnimationFrame({ vCharacter, wCharacter }, Ivec2(1, 2), Ivec2(0, 4), 250))
  vAnim:addFrame(AnimationFrame({ vCharacter, wCharacter }, Ivec2(2, 1), Ivec2(5, 6), 500))
  local vData = {
    a_number = 1234,
    a_string = "Test string",
    a_table = {
      a_nested_string = "Nested string"
    },
    a_character = vCharacter,
    an_object = vObj,
    an_animation = vAnim
  }

  -- Set up the data for the JSON (de)serialisation.
  local jCharacter = Character("j", Colours.BLUE, Colours.WHITE)
  local lCharacter = Character("l", Colours.WHITE, Colours.BLUE)
  local jObj = GameObject(Ivec2(1, 3), Values.IVEC2_ONE)
  jObj.data.data[1] = jCharacter
  local jAnim = Animation("json_anim")
  jAnim:addFrame(AnimationFrame({ jCharacter, lCharacter }, Ivec2(1, 2), Ivec2(3, 4), 250))
  jAnim:addFrame(AnimationFrame({ jCharacter, lCharacter }, Ivec2(2, 1), Ivec2(5, 6), 500))
  local jData = {
    a_number = 5678,
    a_string = "Example string",
    a_table = {
      a_nested_string = "A deeper string"
    },
    a_character = jCharacter,
    an_object = jObj,
    an_animation = jAnim
  }

  -- Serialise the data to vON and JSON.
  local vStr = von.serialize(vData)
  local jStr = json.encode(jData)
  
  print("Serialised vON: " .. vStr)
  print("-----------------")
  print("Serialised JSON: " .. jStr)
  print("-----------------")
  
  -- Destroy the old objects. When the data is deserialised, new objects will be created.
  vObj:release()
  jObj:release()
  
  -- Deserialise the data from vON and JSON.
  vTbl = von.deserialize(vStr)
  jTbl = json.decode(jStr)

  print("Deserialised vON: {")
  printTable(vTbl)
  print("}")
  print("-----------------")
  print("Deserialised JSON: {")
  printTable(jTbl)
  print("}")

  return true
end
