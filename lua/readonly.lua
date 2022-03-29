function readOnly(t)
  local proxy = {}
  local meta = {
    __index = t,
    __newindex = function(t, k, v)
      error("Attempted to edit a read-only table.", 2)
    end
  }
  
  setmetatable(proxy, meta)

  return proxy
end