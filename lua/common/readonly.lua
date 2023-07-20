--[[
-- @author James Holtom
--]]

-- @brief Overrides a table's metatable to return an error when attempting to set properties.
local function newIndexError(_, _, _)
  error("Attempted to set a property on a read-only table.", 2)
end

-- @brief Configures a table's metatable to disallow setting properties.
function readOnly(t)
  local proxy = {}
  local meta = {
    __index = t,
    __newindex = newIndexError
  }
  
  setmetatable(proxy, meta)

  return proxy
end

-- @brief Refers to the original "pairs()" function.
local originalPairs = pairs

--[[
-- @brief An extension of the "pairs()" function, to correctly return values from read-only tables.
-- @param tbl The table to retrieve data from.
--]]
function pairs(tbl)
  if next(tbl) == nil then
    local meta = getmetatable(tbl)

    if meta and meta.__newindex == newIndexError then
      tbl = meta.__index
    end
  end

  return originalPairs(tbl)
end
