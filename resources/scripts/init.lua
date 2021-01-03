function Init()
	print("Init -> Init()")
	
	return true
end

function Loop()
	--print("Init -> Loop()")
	
	if keyIsPressed("m") then
		print(mousePosition())
	end
end

function Quit()
	print("Init -> Quit()")
	
	return true
end
