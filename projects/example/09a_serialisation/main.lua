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
  local glyph = Glyph("a", Colours.RED, Colours.WHITE)
  local obj = Object(Values.VEC2_ONE, Values.IVEC2_ONE)
  obj.active = false
  obj.data[1] = glyph
  local tbl = {
    a_number = 1234,
    a_string = "Test string",
    a_table = {
      a_nested_string = "Nested string"
    },
    a_glyph = glyph,
    an_object = obj
  }

  -- (De)serialise the table using vON.
  local vStr = von.serialize(tbl)
  local vTbl = von.deserialize(vStr)

  -- (De)serialise the table using JSON.
  local jStr = json.encode(tbl)
  local jTbl = json.decode(jStr)

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