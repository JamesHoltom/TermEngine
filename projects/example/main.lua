function Init()
	local glyph_set = activeScene:getGlyphSet()
	local size = glyph_set.size

	for x = 0, size.x - 1 do
		for y = 0, size.y - 1 do
			local glyph = glyph_set:get(vec2u(x,y))
			glyph.character = "B"
			glyph.foreground_color = Color(1 - (x / size.x), 1, 1 - (y / size.y), 1)
			glyph.background_color = Color(1, 1, 1, 0.5)
		end
	end

	glyph_set:refresh()
	
	return true
end

function Loop()
	if mouseIsPressed(1) then
		print("Pressed")
	end
end
