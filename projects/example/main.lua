require("projects/example/player")

local player, outline, text

function Init()
	player = Player(vec2(4))
	
	outline = BoxObject(vec2(0, 0), ivec2(32, 16))
	outline.setOutline(Glyph("+", vec3(0), vec3(255)))
	
	text = TextObject(vec2(1, 14), ivec2(30, 1))
	text.setText("Welcome to TermEngine!")
	text.setColors(vec3(255), vec3(128))
	
	return true
end

function Loop(timestep)
	player.doInput(timestep)
	
	if keyboard.isPressed("r") then
		print("Number 1, 0~1:    " .. rand.get())
		print("Number 2, 0~10:   " .. rand.get(10))
		print("Number 3, 10~100: " .. rand.get(90, 10))
	end
end
