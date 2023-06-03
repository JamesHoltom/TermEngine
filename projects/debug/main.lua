--[[
-- @author James Holtom
--]]

local animObj, anim

function Init()
	defaultScene.window.position.x = 10

	animObj = GameObject(Values.IVEC2_ZERO, ivec2(8, 8))

	local animData = {
		Character("/", Colours.RED, Colours.BLACK), Character("\\", Colours.RED, Colours.BLACK),
		Character("\\", Colours.RED, Colours.BLACK), Character("/", Colours.RED, Colours.BLACK)
	}
	local animFrames = {
		AnimationFrame(animData, ivec2(2), Values.IVEC2_ZERO, 0),
		AnimationFrame(animData, ivec2(2), Values.IVEC2_ONE, 0),
		AnimationFrame(animData, ivec2(2), ivec2(2), 0)
	}

	anim = Animation("test")
	for _, frame in ipairs(animFrames) do
		anim:addFrame(frame)
	end

	animObj.animation:addToQueue({ animation=anim, yoyo=true, frame_rate=166 });

	return true
end
