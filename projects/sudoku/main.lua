require("projects/sudoku/sudoku")

local sudoku

function Init()
	sudoku = GameObject()
	sudoku.startGame()
	
	view.setSize(sudoku.getObjectSize())
	window.fitToView()

	return true
end

function Loop(timestep)
	if keyboard.isPressed("s") then
		sudoku.test()
	end
end

function Quit()
	return true
end