local events

function Init()
  events = {
    mousedown=EventListener("mouse_down", function(_, event) print("Clicked "..tostring(event.clicks).." times, last at "..tostring(event.position)) end),
    keydown=EventListener("key_down", function(_, event) print("Pressed "..event.key) end),
    mousefocus=EventListener("window_mouse_focus", function(_, event) print("Gained mouse focus.") end),
    keyboardfocus=EventListener("window_keyboard_focus", function(_, event) print("Gained keyboard focus.") end),
    mouseblur=EventListener("window_mouse_blur", function(_, event) print("Lost mouse focus.") end),
    keyboardblur=EventListener("window_keyboard_blur", function(_, event) print("Lost keyboard focus.") end),
    windowmoved=EventListener("window_moved", function(_, event) print("Moved window to "..tostring(event.position)) end),
    windowresized=EventListener("window_resized", function(_, event) print("Resized window to "..tostring(event.size)) end)
  }

  return true
end
