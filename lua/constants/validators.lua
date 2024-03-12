--[[
-- @brief Tests if a form control requires a value, that the user has omitted.
-- @returns If the control has a value, or an error message if not.
--]]
function isRequired(_self)
  if _self.required and utf8.len(tostring(_self.value)) == 0 then
    return false, "This field is required!"
  end

  return true, ""
end