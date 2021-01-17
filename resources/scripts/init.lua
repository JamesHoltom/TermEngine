function Init()
	print("Init -> Init()")
	
	return true
end

function Loop()
	--print("Init -> Loop()")
	
	if keyIsPressed("m") then
		print(mousePosition())
	end
	
	if keyIsPressed("s") then
		print(getGlyphSetSize())
	end
	
	if keyIsPressed("d") then
		print(isSetDirty() and "true" or "false")
	end
	
	if keyIsPressed("w") then
		print("Test")
		writeText("Test", ivec2(0, 0), ivec2(10, 0))
		print(isSetDirty() and "true" or "false")
	end
	
	--[[if keyIsPressed("f") then
		getFileList("resources")
	end
	
	if keyIsPressed("g") then
		getFolderList("resources")
	end]]
end

function Quit()
	print("Init -> Quit()")
	
	return true
end
