function Init()
	draw.outlinedBox("#", ivec2(0, 0), ivec2(31, 15), vec4(0, 0, 255, 255), vec4(0, 0, 0, 255))
	draw.box("x", ivec2(1, 5), ivec2(30, 14), vec4(255, 0, 0, 255), vec4(255, 255, 255 ,255))
	draw.text("Hello World!", ivec2(1, 1), ivec2(30, 1), vec4(255, 255, 255, 255), vec4(0, 0, 0, 255))
	
	print("Hello World!")
	
	return true
end

function Loop()
	if mouse.isPressed(mouse.LEFT) then
		print("Pressed")
	end
end
