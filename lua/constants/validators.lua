function isRequired(_self)
  if _self.required and #_self.value == 0 then
    return false, "This field is required!"
  end

  return true, ""
end