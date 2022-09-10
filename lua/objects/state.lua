function State(_name)
  local self = {
    name = _name,
    objects = {},
    active = false,
    count = 0
  }

  local _getName = function()
    return self.name
  end

  local _addObject = function(_name, _obj)
    self.objects[_name] = _obj
    self.objects[_name].setActive(self.active)
    self.count = self.count + 1
  end

  local _removeObject = function(_name)
    if self.objects[_name] ~= nil then
      self.objects[_name]:release()
      self.objects[_name] = nil

      self.count = self.count - 1
    end
  end

  local _getObject = function(_name)
    return self.objects[_name]
  end

  local _countObjects = function()
    return self.count
  end

  local _isActive = function()
    return self.active
  end

  local _setActive = function(_flag)
    self.active = _flag

    for _, obj in pairs(self.objects) do
      obj.setActive(_flag)
    end
  end

  local _release = function(_)
    for _, obj in pairs(self.objects) do
      obj:release()
    end
  end

  return {
    getName = _getName,
    addObject = _addObject,
    removeObject = _removeObject,
    getObject = _getObject,
    countObjects = _countObjects,
    isActive = _isActive,
    setActive = _setActive,
    release = _release
  }
end

function StateManager()
  local self = {
    states = {},
    active = nil,
    count = 0
  }

  local _addState = function(_name)
    self.states[_name] = State(_name)
    self.count = self.count + 1

    if self.count == 1 then
      self.active = _name
      self.states[_name].setActive(true)
    end

    return self.states[_name]
  end

  local _removeState = function(_name)
    if self.states[_name] ~= nil then
      self.states[_name]:release()
      self.states[_name] = nil
      self.count = self.count - 1

      if self.count == 0 then
        self.active = nil
      end
    end
  end

  local _getActiveState = function()
    return self.states[self.active]
  end

  local _getState = function(_name)
    return self.states[_name]
  end

  local _countStates = function()
    return self.count
  end

  local _changeState = function(_name)
    if self.active ~= nil then
      self.states[self.active].setActive(false)
    end

    self.active = _name

    if self.active ~= nil then
      self.states[self.active].setActive(true)
    end
  end

  local _release = function(_)
    for _, state in pairs(self.states) do
      state:release()
    end
  end

  return {
    addState = _addState,
    removeState = _removeState,
    getActiveState = _getActiveState,
    getState = _getState,
    countStates = _countStates,
    changeState = _changeState,
    release = _release
  }
end