local helloWorld

function Init()
  helloWorld = TextObject(vec2(1, 1), ivec2(12, 1))
	helloWorld.setText("Hello World!")

  return true
end
