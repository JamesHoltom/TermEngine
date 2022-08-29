local events

function Init()
  events = {
    mousedown=EventListener("mouse_down", function(self, event) print("Clicked "..tostring(event.clicks).." times, last at "..tostring(event.position)) end),
    keydown=EventListener("key_down", function(self, event) print("Pressed "..event.key) end),
    mousefocus=EventListener("window_mouse_focus", function(self, event) print("Gained mouse focus.") end),
    keyboardfocus=EventListener("window_keyboard_focus", function(self, event) print("Gained keyboard focus.") end),
    mouseblur=EventListener("window_mouse_blur", function(self, event) print("Lost mouse focus.") end),
    keyboardblur=EventListener("window_keyboard_blur", function(self, event) print("Lost keyboard focus.") end),
    windowmoved=EventListener("window_moved", function(self, event) print("Moved window to "..tostring(event.position)) end),
    windowresized=EventListener("window_resized", function(self, event) print("Resized window to "..tostring(event.size)) end)
  }

  for k, v in ipairs(event.getListenerTypes()) do
    print(v..": "..event.countListeners(v))
  end

  return true
end

function Quit()
  for k, v in pairs(events) do
    v:release();
  end

  return true
end