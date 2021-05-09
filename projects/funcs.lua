function addBox(name, position, size, content)
	local box = objects.add(name, position, size)
	
	for k = 1, #box.data do
		box.data[k] = content
	end
	
	return box
end

function addOutlinedBox(name, position, size, content)
	local outline = objects.add(name, position, size)
	
	for top = 1, size.x do
		outline.data[top] = content
	end
	
	for bottom = #outline.data - size.x + 1, #outline.data do
		outline.data[bottom] = content
	end
	
	for left = 1, #outline.data - size.x + 1, size.x do
		outline.data[left] = content
	end
	
	for right = size.x, #outline.data, size.x do
		outline.data[right] = content
	end
	
	return outline
end

function addText(name, position, size, text, foreground_color, background_color)
	local textbox = objects.add(name, position, size)
	local index = 1
	
	for w in text:gmatch(".") do
		if index > #textbox.data then break end
		
		textbox.data[index] = Glyph(w, foreground_color, background_color)
		index = index + 1
	end
	
	return textbox
end
