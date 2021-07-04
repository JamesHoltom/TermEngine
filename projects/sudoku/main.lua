require("projects/sudoku/sudoku")

local sudoku, main_menu, sudoku_menu

function Init()
	sudoku = GameObject()
	main_menu = MenuObject()
	sudoku_menu = MenuObject()
	
	sudoku.init()
	
	main_menu.setOptionSize(ivec2(10, 1))
	main_menu.addOption(1, MenuOption("New Game", function()
		main_menu.setActive(false)
		sudoku.startGame()
		
		view.setSize(sudoku.getObjSize())
		window.fitToView()
	end))
	main_menu.addOption(2, MenuOption("Load Game", function() print("Load Game!") end))
	main_menu.addOption(3, MenuOption("Credits", function() print("Credits!") end))
	
	--sudoku_menu.addOption(1, {}) -- Grid Type
	--sudoku_menu.addOption(2, {}) -- Grid Size
	--sudoku_menu.addOption(3, {}) -- Rules
	sudoku_menu.setActive(false)

	return true
end

function OnKeyPress(key)
	main_menu.doKeyInput(key)
	sudoku.doKeyInput(key)
end
