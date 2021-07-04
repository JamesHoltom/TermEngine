function Player(_pos)
	local self = {
		box = BoxObject(_pos, ivec2(2)),
		speed = 9
	}
	
	self.box.setFill(Glyph("o", vec3(255, 0, 0), vec3(0)))
	
	local doInput = function(_timestep)
		if mouse.isPressed(mouse.LEFT) then
			if self.box.getFill().character == "o" then
				self.box.setFill(Glyph("x", vec3(0, 0, 255), vec3(0)))
			else
				self.box.setFill(Glyph("o", vec3(255, 0, 0), vec3(0)))
			end
		end
		
		if mouse.isPressed(mouse.RIGHT) then
			if self.box.hasOutline() then
				self.box.setOutline(empty_glyph)
			else
				self.box.setOutline(Glyph("*", vec3(0, 255, 0), vec3(255)))
			end
		end
		
		local pos = self.box.getPosition()
		local has_moved = false
		
		if keyboard.isDown("left") then
			pos.x = pos.x - (self.speed * _timestep)
			has_moved = true
		end
		
		if keyboard.isDown("right") then
			pos.x = pos.x + (self.speed * _timestep)
			has_moved = true
		end
		
		if keyboard.isDown("up") then
			pos.y = pos.y - (self.speed * _timestep)
			has_moved = true
		end
		
		if keyboard.isDown("down") then
			pos.y = pos.y + (self.speed * _timestep)
			has_moved = true
		end
		
		if keyboard.isReleased("left") or keyboard.isReleased("right") or keyboard.isReleased("up") or keyboard.isReleased("down") then
			pos = pos:floor() + vec2(0.5)
			has_moved = true
		end
		
		if has_moved then
			self.box.setPosition(pos)
		end
		
		if keyboard.isPressed("space") then
			if self.box.getSize() == ivec2(2) then
				self.box.setSize(ivec2(1))
			else
				self.box.setSize(ivec2(2))
			end
		end
	end
	
	return {
		doInput = doInput
	}
end
