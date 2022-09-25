local vTbl, jTbl

function printTable(table, depth)
  depth = depth or 1

  local tab = string.rep(" ", depth * 2)

  for k,v in pairs(table) do
    if type(v) == "table" then
      print(tab .. k .. ": {")
      printTable(v, depth + 1)
      print(tab .. "}")
    elseif type(v) == "userdata" then
      if v.__type.name == "term_engine::objects::Object" then
        print(tab .. k .. ": Object - Pos(" .. tostring(v.position) .. "), Size(" .. tostring(v.size) .. "), Active(" .. tostring(v.active) .. "), Data([")

        for i, j in ipairs(v.data) do
          print(tab .. tab .. i .. ": Glyph - Char(" .. j.character .. "), Foreground(" .. tostring(j.foreground_colour) .. "), Background(" .. tostring(j.background_colour) .. ")")
        end

        print(tab .. "])")
      elseif v.__type.name == "term_engine::resources::GlyphParams" then
        print(tab .. k .. ": Glyph - Char(" .. v.character .. "), Foreground(" .. tostring(v.foreground_colour) .. "), Background(" .. tostring(v.background_colour) .. ")")
      end
    else
      print(tab .. k .. ": " .. tostring(v))
    end
  end
end

function Init()
  -- Set up the data for the vON (de)serialisation.
  local vGlyph = Glyph("v", Colours.RED, Colours.WHITE)
  local vObj = Object(Values.VEC2_ONE, Values.IVEC2_ONE)
  vObj.data[1] = vGlyph
  local vData = {
    a_number = 1234,
    a_string = "Test string",
    a_table = {
      a_nested_string = "Nested string"
    },
    a_glyph = vGlyph,
    an_object = vObj
  }

  -- Set up the data for the JSON (de)serialisation.
  local jGlyph = Glyph("j", Colours.BLUE, Colours.WHITE)
  local jObj = Object(vec2(1, 3), Values.IVEC2_ONE)
  jObj.data[1] = jGlyph
  local jData = {
    a_number = 5678,
    a_string = "Example string",
    a_table = {
      a_nested_string = "A deeper string"
    },
    a_glyph = jGlyph,
    an_object = jObj
  }

  -- Serialise the data to vON and JSON.
  local vStr = von.serialize(vData)
  local jStr = json.encode(jData)

  -- Destroy the old objects. When the data is deserialised, new objects will be created.
  vObj:release()
  jObj:release()

  -- Deserialise the data from vON and JSON.
  vTbl = von.deserialize(vStr)
  jTbl = json.decode(jStr)

  print("Serialised vON: " .. vStr)
  print("-----------------")
  print("Deserialised vON: {")
  printTable(vTbl)
  print("}")
  print("-----------------")
  print("Serialised JSON: " .. jStr)
  print("-----------------")
  print("Deserialised JSON: {")
  printTable(jTbl)
  print("}")

  return true
end

function Quit()
  vTbl.an_object:release()
  jTbl.an_object:release()

  return true
end