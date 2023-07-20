--[[
-- @author James Holtom
--]]

local projectText

function Init()
  projectText = TextObject(Values.IVEC2_ONE, Ivec2(18, 3), "This is Project 2!Press 'p' to go tothe other project!")
  
  return true
end

function Loop(_)
  if keyboard.isPressed("p") then
    project.load("examples/multiple_projects/project_1")
  end
end