local default_text_color = vec3(255)
local default_bg_color = vec3(0)

function Init()
	print("Init -> Init()")
	
	addOutlinedBox("no_prg_outline", vec2(0), ivec2(32, 16), Glyph("#", default_text_color, default_bg_color))
	addText("no_prg_text", vec2(11, 7), ivec2(21, 7), "No Program", default_text_color, default_bg_color)
	
	return true
end

function Loop(timestep)
	
end

function Quit()
	print("Init -> Quit()")
	
	return true
end
