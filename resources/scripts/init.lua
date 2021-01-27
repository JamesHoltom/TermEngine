local default_text_color = Color(255)
local default_bg_color = Color(0, 0, 0, 255)

function Init()
	print("Init -> Init()")
	
	writeText("No Program", ivec2(11, 7), ivec2(21, 7), default_text_color, default_bg_color)
	drawOutlinedBox("#", ivec2(0, 0), ivec2(32, 16), default_text_color, default_bg_color)
	
	return true
end

function Loop()
	
end

function Quit()
	print("Init -> Quit()")
	
	return true
end
