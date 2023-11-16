--[[
-- @author James Holtom
--]]

local projectText

function Init()
  projectText = TextObject(Values.IVEC2_ONE, Ivec2(20, 3), "This is Project 2!\nPress 'p' to go to\nthe other project!")
  
  return true
end

function Loop(_)
  if keyboard.isPressed("p") then
    project.load("examples/multiple_projects/project_1")
  end
end