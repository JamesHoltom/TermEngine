local posText, posEvent

function Init()
  posText = TextObject(Values.IVEC2_ONE, ivec2(24, 3))
  posEvent = EventListener("mouse_move", function(_, event)
    posText.text = "Index:      " .. tostring(getIndexFromPosition(view.getSize(), event.position)) .. "\n" ..
                   "Row/Column: " .. tostring(getRowColFromPosition(event.position)) .. "\n" ..
                   "Position:   " .. tostring(event.position)
  end)
  
  return true
end
