local box, outline, textbox

function Init()
	outline = object.addOutlinedBox(ivec2(0, 0), ivec2(31, 15), Glyph("#", vec4(0, 0, 255, 255), vec4(0, 244, 255, 255)))
	box = object.addBox(ivec2(1, 1), ivec2(30, 14), Glyph("-", vec4(255, 224, 0, 255), vec4(128, 128, 0, 255)))
	textbox = object.addText(ivec2(3, 3), ivec2(20, 6), "Hello World!", vec4(255, 255, 255, 255), vec4(0, 0, 0, 255))
	sprite = object.addBox(ivec2(5, 5), ivec2(6, 6), Glyph("O", vec4(0, 0, 0, 255), vec4(255)))
	
	print("Hello World!")
	
	return true
end

function Loop(timestep)
	if mouse.isPressed(mouse.LEFT) then
		textbox.text = "This\nis\na\ntest!"
		textbox.foreground_color = vec4(0, 255, 0, 255)
		textbox.background_color = vec4(0, 0, 255, 255)
		print("Pressed")
	end
	
	if mouse.isPressed(mouse.RIGHT) then
		print(mouse.position.x .. ", " .. mouse.position.y)
	end

	local pos = sprite.position
	local speed = 4 * timestep
	
	if keyboard.isDown("left") then
		pos.x = pos.x - speed
	end
	
	if keyboard.isDown("right") then
		pos.x = pos.x + speed
	end
	
	if keyboard.isDown("up") then
		pos.y = pos.y - speed
	end
	
	if keyboard.isDown("down") then
		pos.y = pos.y + speed
	end
	
	sprite.position = pos
end
