local default_text_color = vec3(255)
local default_bg_color = vec3(0)

local outline, noProgramText

function Init()
	print("Init -> Init()")
	
	outline = BoxObject(vec2(0), ivec2(32, 16))
	outline.setOutline(Glyph("#", default_text_color, default_bg_color))

	noProgramText = TextObject(vec2(11, 7), ivec2(21, 7))
	noProgramText.setText("No Program")
	noProgramText.setColors(default_text_color, vec3(255, 0, 0))

	return true
end

function Loop(timestep)
	
end

function Quit()
	print("Init -> Quit()")
	
	return true
end
