local box, outline, textbox

function Init()
	box = BoxObject("test_box", vec2(1, 1), ivec2(4, 4))
	box.setFill(Glyph("o", vec3(255, 0, 0), vec3(0)))
	
	outline = BoxObject("test_outline", vec2(0, 0), ivec2(32, 16))
	outline.setOutline(Glyph("+", vec3(0), vec3(255)))
	
	text = TextObject("test_text", vec2(1, 14), ivec2(30, 1))
	text.setText("Welcome to TermEngine!")
	text.setColors(vec3(255), vec3(128))
	
	return true
end

function Loop(timestep)
	if mouse.isPressed(mouse.LEFT) then
		print("Pressed left mouse")
		
		if box.getFill().character == "o" then
			box.setFill(Glyph("x", vec3(0, 0, 255), vec3(0)))
		else
			box.setFill(Glyph("o", vec3(255, 0, 0), vec3(0)))
		end
	end
	
	if mouse.isPressed(mouse.RIGHT) then
		print("Pressed right mouse")
		
		if box.hasOutline() then
			box.setOutline(empty_glyph)
		else
			box.setOutline(Glyph("*", vec3(0, 255, 0), vec3(255)))
		end
	end

	local speed = 9 * timestep
	local pos = box.getPosition()
	
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
	
	box.setPosition(pos)
	
	if keyboard.isPressed("space") then
		local size = box.getSize()
		
		if size == ivec2(4) then
			box.setSize(ivec2(6))
		else
			box.setSize(ivec2(4))
		end
	end
end
