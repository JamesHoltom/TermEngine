local outline, noProgramText

function Init()
	print("Init -> Init()")
	
	outline = BoxObject(vec2(0), ivec2(32, 16))
	outline.setOutline(Glyph("#", Colours.WHITE, Colours.BLACK))

	noProgramText = TextObject(vec2(11, 7), ivec2(21, 7))
	noProgramText.setText("No Program")
	noProgramText.setColours(Colours.WHITE, Colours.RED)

	window.fitToView()

	return true
end

function Quit()
	print("Init -> Quit()")
	
	return true
end
