local text

function Init()
  font.tabSize = 4

  text = TextObject(Values.VEC2_ONE, ivec2(30, 6))
  text.text = "Writing text!\n\nNewlines are supported!\n\n\tTabs are supported!"

  return true
end

function Quit()
  text:release()

  return true
end