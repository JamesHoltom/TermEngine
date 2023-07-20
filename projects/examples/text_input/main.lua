--[[
-- @author James Holtom
--]]

local inputText = ""
local inputActiveObj, inputTextObj, keyListener, textInputListener

function Init()
	inputActiveObj = TextObject(Values.IVEC2_ZERO, Ivec2(13, 1), "Active: "..tostring(textInput.isActive()))
	inputTextObj = TextObject(Ivec2(0, 1), Ivec2(32, 1), "Text: ")
	keyListener = EventListener("key_down", function(_, data)
		if data.key == "Return" then
			if not textInput.isActive() then
				textInput.start()
			else
				textInput.stop()
			end
	
			inputActiveObj.text = "Active: "..tostring(textInput.isActive())
		elseif data.key == "Backspace" and #inputText > 0 then
			local offset = utf8.offset(inputText, -1)

			if offset then
				inputText = inputText:sub(1, offset - 1)

				inputTextObj.text = "Text: "..inputText
			end
		elseif data.key == "C" and data.ctrl then
			clipboard.set(inputText)
		elseif data.key == "V" and data.ctrl then
			inputText = clipboard.get()

			inputTextObj.text = "Text: "..inputText
		end
	end)
	textInputListener = EventListener("text_input", function(_, data)
		if utf8.len(inputText) < 26 then
			inputText = inputText .. data.text

			inputTextObj.text = "Text: "..inputText
		end
	end)

	return true
end
