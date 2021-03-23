local default_text_color = vec4(255)
local default_bg_color = vec4(0, 0, 0, 255)

function Init()
	print("Init -> Init()")
	
	draw.outlinedBox("#", ivec2(0, 0), ivec2(31, 15), default_text_color, default_bg_color)
	draw.text("No Program", ivec2(11, 7), ivec2(21, 7), default_text_color, default_bg_color)
	
	return true
end

function Loop()
	
end

function Quit()
	print("Init -> Quit()")
	
	return true
end
