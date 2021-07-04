function GridOptions()
	local self = {
		box_size  = ivec2(3),
		grid_size = ivec2(9)
	}
	
	local _setSize = function(size)
		if size >= 2 and size <= 8 then
			self.box_size = ivec2(size)
			self.grid_size = ivec2(size * size)
		else
			print("Invalid size given!")
		end
	end
	
	local _getBoxSize = function()
		return self.box_size
	end
	
	local _getGridSize = function()
		return self.grid_size
	end
	
	return {
		getBoxSize      = _getBoxSize,
		getGridSize     = _getGridSize,
		setSize         = _setSize
		--is_jigsaw_grid  = false,
		--is_killer_grid  = false,
		--is_samurai_grid = false,
		--is_center_grid  = false,
		--is_hyper_grid   = false
	}
end

function RuleSet()
	return {
		--diagonal_rule    = false,
		--odd_even_rule    = false,
		--consecutive_rule = false,
		--xv_rule          = false,
		--arrow_rule       = false,
		--thermo_rule      = false,
		--killer_math_rule = false
	}
end

function GameObject()
	local self = {
		obj = Object(vec2(), ivec2(1)),
		in_game = false,
		difficulty = 0,
		last_highlighted_cell = ivec2(),
		highlighted_cell = ivec2(),
		grid_options = GridOptions(),
		rule_set = RuleSet(),
		initial_grid = {},
		played_grid = {}
	}
	
	local _getObjSize = function()
		return self.obj.size
	end
	
	local _positionToIndex = function(position)
		local offset = position / self.grid_options.getBoxSize()
		
		return ((position.y + offset.y) * self.obj.size.x) + position.x + offset.x + 1
	end
	
	local _validSudokuValue = function(key)
		return tonumber(key) ~= nil and tonumber(key) > 0 and tonumber(key) <= 9
	end
	
	-- NB: Spaces are used here, rather than NO_CHARACTER, so that the background color is still visible.
	local _drawGridOutline = function()
		local x = 0
		local y = 0
		local chr = " "
		
		for i = 1, #self.obj.data do
			x = ((i - 1) % self.obj.size.x) + 1
			y = math.floor((i - 1) / self.obj.size.y) + 1
			
			if y % (self.grid_options.getBoxSize().y + 1) == 0 then
				if x % (self.grid_options.getBoxSize().x + 1) == 0 then
					chr = "+"
				else
					chr = "-"
				end
			else
				if x % (self.grid_options.getBoxSize().x + 1) == 0 then
					chr = "|"
				else
					chr = " "
				end
			end
			
			self.obj.data[i] = Glyph(chr, Colors.white, Colors.black)
		end
	end
	
	local _updateGrid = function()
		self.obj.size = self.grid_options.getGridSize() + self.grid_options.getBoxSize() - ivec2(1)
	end
	
	local _highlightCell = function()
		self.obj.data[_positionToIndex(self.last_highlighted_cell)].background_color = Colors.black
		self.obj.data[_positionToIndex(self.highlighted_cell)].background_color = Colors.green
		
		self.last_highlighted_cell.x = self.highlighted_cell.x
		self.last_highlighted_cell.y = self.highlighted_cell.y
		
		objects.dirty()
	end
	
	local _setCell = function(value)
		self.obj.data[_positionToIndex(self.highlighted_cell)].character = value
		
		objects.dirty()
	end
	
	-- NB: Spaces are used here, rather than NO_CHARACTER, so that the background color is still visible.
	local _clearCell = function()
		self.obj.data[_positionToIndex(self.highlighted_cell)].character = " "
		
		objects.dirty()
	end
	
	local _generateGame = function()
		
	end
	
	local _solveGame = function()
		
	end
	
	local _startGame = function()
		self.obj.active = true
		
		_updateGrid()
		_drawGridOutline()
		_highlightCell()
		
		objects.dirty()
	end
	
	local _endGame = function()
		self.obj.active = false
	end
	
	local _resetGame = function()
		
	end
	
	local _setGridOptions = function(_options)
		
	end
	
	local _setRuleSet = function(_rules)
		
	end
	
	local _init = function()
		self.obj.active = false
	end
	
	local _doKeyInput = function(key)
		if self.obj.active then
			if key == "Left" and self.highlighted_cell.x > 0 then
				self.highlighted_cell.x = self.highlighted_cell.x - 1
			end
			
			if key == "Right" and self.highlighted_cell.x < self.grid_options.getGridSize().x - 1 then
				self.highlighted_cell.x = self.highlighted_cell.x + 1
			end
			
			if key == "Up" and self.highlighted_cell.y > 0 then
				self.highlighted_cell.y = self.highlighted_cell.y - 1
			end
			
			if key == "Down" and self.highlighted_cell.y < self.grid_options.getGridSize().y - 1 then
				self.highlighted_cell.y = self.highlighted_cell.y + 1
			end
			
			if self.highlighted_cell ~= self.last_highlighted_cell then
				_highlightCell()
			end
			
			if _validSudokuValue(key) then
				_setCell(key)
			elseif key == "0" or key == "Backspace" then
				_clearCell()
			end
		end
		
		if key == "G" then
			self.obj.active = not self.obj.active
			objects.dirty()
		end
	end
	
	return {
		init       = _init,
		doKeyInput = _doKeyInput,
		getObjSize = _getObjSize,
		startGame  = _startGame
	}
end
