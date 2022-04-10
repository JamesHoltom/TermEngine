local function newIndexError(t, k, v)
  error("Attempted to edit a read-only table.", 2)
end

function readOnly(t)
  local proxy = {}
  local meta = {
    __index = t,
    __newindex = newIndexError
  }
  
  setmetatable(proxy, meta)

  return proxy
end

local originalPairs = pairs

function pairs(t)
  if next(t) == nil then
    local meta = getmetatable(t)

    if meta and meta.__newindex == newIndexError then
      t = meta.__index
    end
  end

  return originalPairs(t)
end
