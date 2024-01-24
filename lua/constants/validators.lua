function isRequired(_self)
  if _self.required and utf8.len(tostring(_self.value)) == 0 then
    return false, "This field is required!"
  end

  return true, ""
end