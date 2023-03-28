function Player(_pos)
	local self = {
		box = BoxObject(_pos, ivec2(2)),
		pos = vec2(_pos),
		speed = 9
	}
	
	self.box.fill = Character("o", Colours.RED, Colours.BLACK)
	
	local _update = function(_, timestep)
		if mouse.isPressed(mouse.LEFT) then
			if self.box.fill.character == "o" then
				self.box.fill = Character("x", Colours.BLUE, Colours.BLACK)
			else
				self.box.fill = Character("o", Colours.RED, Colours.BLACK)
			end
		end
		
		if mouse.isPressed(mouse.RIGHT) then
			if self.box.hasOutline() then
				self.box.outline = Character("", Colours.BLACK, Colours.WHITE)
			else
				self.box.outline = Character("*", Colours.GREEN, Colours.WHITE)
			end
		end
		
		local hasMoved = false
		
		if keyboard.isDown("Left") then
			self.pos.x = self.pos.x - (self.speed * timestep / 1000)
			hasMoved = true
		end
		
		if keyboard.isDown("Right") then
			self.pos.x = self.pos.x + (self.speed * timestep / 1000)
			hasMoved = true
		end
		
		if keyboard.isDown("Up") then
			self.pos.y = self.pos.y - (self.speed * timestep / 1000)
			hasMoved = true
		end
		
		if keyboard.isDown("Down") then
			self.pos.y = self.pos.y + (self.speed * timestep / 1000)
			hasMoved = true
		end
		
		if hasMoved then
			self.box.position = ivec2(self.pos)
		end
		
		if keyboard.isPressed("Space") then
			if self.box.size == ivec2(2) then
				self.box.size = ivec2(1)
			else
				self.box.size = ivec2(2)
			end
		end
	end
	
	return {
		update = _update
	}
end
