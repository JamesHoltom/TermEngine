local box, outline, textbox

function Init()
	print("Hello World!")
	print(_VERSION)
	
	box = addBox("test_box", vec2(1, 1), ivec2(2, 2), Glyph("#", vec3(255, 0, 0), vec3(0)))
	outline = addOutlinedBox("test_outline", vec2(0, 0), ivec2(32, 16), Glyph("+", vec3(0), vec3(255)))
	text = addText("test_text", vec2(1, 14), ivec2(30, 1), "Welcome to TermEngine!", vec3(255), vec3(128))
	
	return true
end

function Loop(timestep)
	if mouse.isPressed(mouse.LEFT) then
		print("Pressed")
	end
	
	if mouse.isPressed(mouse.RIGHT) then
		print(mouse.position.x .. ", " .. mouse.position.y)
	end

	local speed = 9 * timestep
	local pos = box.position
	
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
	
	if keyboard.isReleased("left") or keyboard.isReleased("right") or keyboard.isReleased("up") or keyboard.isReleased("down") then
		pos = pos:floor() + vec2(0.5)
	end
	
	box.position = pos
end
