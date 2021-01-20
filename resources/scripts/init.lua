function Init()
	print("Init -> Init()")
	
	local folders = getFolderList("resources")
	local folderStr = ""
	
	for k,v in ipairs(folders) do
		folderStr = folderStr .. tostring(v) .. "\n"
	end
	
	writeText(folderStr, ivec2(1, 1), ivec2(31, 15))
	drawOutlinedBox("#", Color(255), Color(0, 0, 0, 255), ivec2(0, 0), ivec2(32, 16))
	
	return true
end

function Loop()
	if keyIsPressed("m") then
		print(mousePosition())
	end
	
	if keyIsPressed("s") then
		print(getGlyphSetSize())
	end
end

function Quit()
	print("Init -> Quit()")
	
	return true
end
