function Player(_pos)
	local self = {
		box = BoxObject(_pos, ivec2(2)),
		pos = vec2(_pos),
		speed = 9
	}
	
	self.box.fill = Character("o", vec3(255, 0, 0), vec3(0))
	
	local doInput = function(_timestep)
		if mouse.isPressed(mouse.LEFT) then
			if self.box.fill.character == "o" then
				self.box.fill = Character("x", vec3(0, 0, 255), vec3(0))
			else
				self.box.fill = Character("o", vec3(255, 0, 0), vec3(0))
			end
		end
		
		if mouse.isPressed(mouse.RIGHT) then
			if self.box.hasOutline() then
				self.box.outline = empty_character
			else
				self.box.outline = Character("*", vec3(0, 255, 0), vec3(255))
			end
		end
		
		local has_moved = false
		
		if keyboard.isDown("left") then
			self.pos.x = self.pos.x - (self.speed * _timestep)
			has_moved = true
		end
		
		if keyboard.isDown("right") then
			self.pos.x = self.pos.x + (self.speed * _timestep)
			has_moved = true
		end
		
		if keyboard.isDown("up") then
			self.pos.y = self.pos.y - (self.speed * _timestep)
			has_moved = true
		end
		
		if keyboard.isDown("down") then
			self.pos.y = self.pos.y + (self.speed * _timestep)
			has_moved = true
		end
		
		if has_moved then
			self.box.position = ivec2(self.pos)
		end
		
		if keyboard.isPressed("space") then
			if self.box.size == ivec2(2) then
				self.box.size = ivec2(1)
			else
				self.box.size = ivec2(2)
			end
		end
	end
	
	return {
		doInput = doInput
	}
end
