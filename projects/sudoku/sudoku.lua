function GridOptions()
	return {
		grid_size = ivec2(3),
		--is_jigsaw_grid = false,
		--is_killer_grid = false,
		--is_cross_grid = false,
		--is_center_grid = false,
		--is_hyper_grid = false
	}
end

function RuleSet()
	return {
		--diagonal_rule = false,
		--odd_even_rule = false,
		--consecutive_rule = false,
		--xv_rule = false,
		--arrow_rule = false,
		--thermo_rule = false,
		--killer_math_rule = false
	}
end

function GameObject()
	local self = {
		obj = Object(vec2(0.0), ivec2(1)),
		in_game = false,
		difficulty = 0,
		grid_options = GridOptions(),
		rule_set = RuleSet(),
		initial_grid = {},
		played_grid = {}
	}
	
	local _drawGridOutline = function()
		local x = 0
		local y = 0
		
		for i = 1, #self.obj.data do
			x = ((i - 1) % self.obj.size.x) + 1
			y = math.floor((i - 1) / self.obj.size.y) + 1
			
			if y % 2 == 1 then
				if x % 2 == 1 then
					self.obj.data[i] = Glyph("+", Colors.white, Colors.black)
				else
					self.obj.data[i] = Glyph("-", Colors.white, Colors.black)
				end
			else
				if x % 2 == 1 then
					self.obj.data[i] = Glyph("|", Colors.white, Colors.black)
				end
			end
		end
	end
	
	local _updateGrid = function()
		self.obj.size = (self.grid_options.grid_size * self.grid_options.grid_size * 2) + ivec2(1)
	end
	
	local _generateGame = function()
		
	end
	
	local _solveGame = function()
		
	end
	
	local _startGame = function()
		_updateGrid()
		_drawGridOutline()
	end
	
	local _resetGame = function()
		
	end
	
	local _setGridOptions = function(_options)
		
	end
	
	local _setRuleSet = function(_rules)
		
	end
	
	local _doInput = function()
		
	end
	
	local _getObjectSize = function()
		return self.obj.size
	end
	
	local _test = function()
		for i = 1, #self.obj.data do
			self.obj.data[i].background_color = Colors.red
		end
	end
	
	return {
		startGame = _startGame,
		doInput   = _doInput,
		getObjectSize = _getObjectSize,
		test = _test
	}
end
