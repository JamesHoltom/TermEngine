local box, outline, textbox

function Init()
	outline = object.addOutlinedBox(ivec2(0, 0), ivec2(31, 15), Glyph("#", vec4(0, 0, 255, 255), vec4(0, 244, 255, 255)))
	box = object.addBox(ivec2(1, 1), ivec2(30, 14), Glyph("-", vec4(255, 224, 0, 255), vec4(128, 128, 0, 255)))
	textbox = object.addText(ivec2(3, 3), ivec2(20, 4), "Hello World!", vec4(255, 255, 255, 255), vec4(0, 0, 0, 255))
	
	print("Hello World!")
	
	return true
end

function Loop()
	if mouse.isPressed(mouse.LEFT) then
		box.position = ivec2(2, 2)
		print("Pressed")
	end

end
