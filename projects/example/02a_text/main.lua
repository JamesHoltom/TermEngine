function Init()
  view.setSize(ivec2(32, 8))
  window.fitToView()
  font.tabSize = 4

  local text = TextObject(Values.VEC2_ONE, ivec2(30, 6))
  text.setText("Writing text!\n\nNewlines are supported!\n\n\tTabs are supported!")

  return true
end