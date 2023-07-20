--[[
-- @author James Holtom
--]]

--[[
-- The purpose of this file is to allow multiple extensions of GameObjects to share the same interface for things like:
--	* Selecting an input field to type into.
--	* Selecting options on a menu.
--
-- To implement this in your custom GameObject:
--	1. Call "SetInteractive()" on your object. Don't forget to call "UnsetInteractive()" when releasing your object.
--	2. Add any of "doHoverEvent()", "doClickEvent()", "doKeyEvent()" or "doTextEvent()" to your object.
--
-- If your object contains any of the above functions, they will be called by the event listeners, being passed the event data.
--]]

local is_init = false					-- @brief Have the event listeners been initialised?
local events = {}							-- @brief The list of event listeners.
local objects = {}						-- @brief The list of interactive objects.
local selected_object = false	-- @brief The currently selected object.

-- @brief Initialises the event listeners for updating interactive objects.
local function InitInteractableEvents()
	if is_init == false then
		events = {
			hover = EventListener("object_hover", function(_, _data)
				for k, v in ipairs(objects) do
					if type(v.doHoverEvent) == "function" then
						objects[k].doHoverEvent()
					end
				end
			end),
			click = EventListener("mouse_down", function(_, _data)
        selected_object = false

				for k, v in ipairs(objects) do
					if _data.rowcol >= v.position and _data.rowcol < (v.position + v.size) then
						selected_object = v.id
					end

          if type(v.doClickEvent) == "function" then
					  objects[k].doClickEvent(_data)
          end
				end

        if selected_object ~= false then
          textInput.start()
        else
          textInput.stop()
        end
			end),
			key = EventListener("key_down", function(_, _data)
				for k, v in ipairs(objects) do
					if type(v.doKeyEvent) == "function" then
						objects[k].doKeyEvent(_data)
					end
				end
			end),
			text = EventListener("text_input", function(_, _data)
				for k, v in ipairs(objects) do
					if type(v.doTextEvent) == "function" then
						objects[k].doTextEvent(_data)
					end
				end
			end)
		}
		is_init = true
	end
end

--[[
-- @brief Adds the given object to the list, marking it as interactive.
-- @param _object The GameObject to add.
-- @returns The GameObject.
--]]
function SetInteractive(_object)
  table.insert(objects, _object)
  InitInteractableEvents()

  return objects[#objects]
end

--[[
-- @brief Removes the interactive object from the list.
-- @param _object The GameObject to remove.
--]]
function UnsetInteractive(_object)
  for k, v in ipairs(objects) do
    if v.id == _object.id then
      objects:remove(k)
      return
    end
  end
end

--[[
-- @brief Returns the interactive object with the given ID.
-- @returns The object, or nil if the ID was not found.
--]]
function GetObject(_id)
	for k, v in ipairs(objects) do
		if v.id == _id then
			return objects[k]
		end
	end

	return nil
end

--[[
-- @brief Returns the number of interactive objects in the list.
-- @returns The count of objects in the list.
--]]
function GetObjectCount()
	return #objects
end

--[[
-- @brief Returns the ID of the currently selected object.
-- @returns The selected object ID, or false if no object is selected.
--]]
function GetSelectedObject()
  return selected_object
end

--[[
-- @brief Marks an object with the given ID as selected.
-- @param _id The ID of the object to select.
-- @notes If the ID cannot be found, the selected object ID will not change.
--]]
function SetSelectedObject(_id)
	for _, v in ipairs(objects) do
		if v.id == _id then
			selected_object = v.id
			return
		end
	end
end