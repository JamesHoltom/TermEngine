--[[
    This project uses audio from OpenGameArt.org, and are distributed under the CC0 license.

    - 8bit SFX, by celestialghost8: https://opengameart.org/content/8bit-sfx
    - NES Shooter Music, by SketchyLogic: https://opengameart.org/content/nes-shooter-music-5-tracks-3-jingles

    CC0 license: https://creativecommons.org/publicdomain/zero/1.0/
  ]]

local audioText, audioMusic

function Init()
  view.setSize(ivec2(26, 3))
  window.fitToView()

  audioText = TextObject(Values.VEC2_ZERO, ivec2(26, 3))
  audioText.setText("Press 's' to play sound\nPress 'm' to play music\nPress 'p' to pause music")

  audioMusic = Audio("resources/audio/Map.wav", "stream")

  return true
end

function Loop(timestep)
  if keyboard.isPressed("s") then
    audio.trigger("resources/audio/1.wav")
  elseif keyboard.isPressed("m") then
    if audioMusic.playing then
      print("Stop")
      audioMusic:stop()
    else
      print("Play")
      audioMusic:play()
    end
  elseif keyboard.isPressed("p") then
    if audioMusic.paused then
      print("Resume")
      audioMusic:resume()
    elseif audioMusic.playing then
      print("Pause")
      audioMusic:pause()
    end
  end
end