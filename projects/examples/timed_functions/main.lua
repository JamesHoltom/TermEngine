local func

function Init()
  func = TimedFunction(3000, true, true, function(self, delay)
    print("This function waited "..delay.."ms to call, and has repeated "..self.timesRepeated.." time(s).")
  end)

  return true
end