--[[
    This project uses audio from OpenGameArt.org, and are distributed under the CC0 license.

    - 8bit SFX, by celestialghost8: https://opengameart.org/content/8bit-sfx
    - NES Shooter Music, by SketchyLogic: https://opengameart.org/content/nes-shooter-music-5-tracks-3-jingles

    CC0 license: https://creativecommons.org/publicdomain/zero/1.0/
  ]]

local audioText, cursorText, settingsText, audioMusic

function Init()
  view.setSize(ivec2(42, 5))
  window.fitToView()

  audioText = TextObject(Values.IVEC2_ZERO, ivec2(26, 4))
  audioText.text = "Press 'n' to play sound\nPress 'm' to play music\nPress 'p' to pause music\nPress 'l' to loop music"

  cursorText = TextObject(ivec2(0.0, 4.0), ivec2(42, 1))
  settingsText = TextObject(ivec2(28.0, 0.0), ivec2(14, 4))

  audioMusic = Audio("resources/audio/Map.wav", "stream")

  print("Length: " .. audioMusic.length)

  return true
end

function Loop(timestep)
  local settingsChanged = true

  if keyboard.isPressed("n") then
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
  elseif keyboard.isPressed("q") then
    settingsChanged = true

    audioMusic.volume = math.min(audioMusic.volume + 0.1, 2.0)
  elseif keyboard.isPressed("a") then
    settingsChanged = true

    audioMusic.volume = math.max(audioMusic.volume - 0.1, 0.0)
  elseif keyboard.isPressed("w") then
    settingsChanged = true

    audioMusic.pan = math.min(audioMusic.pan + 0.1, 1.0)
  elseif keyboard.isPressed("s") then
    settingsChanged = true

    audioMusic.pan = math.max(audioMusic.pan - 0.1, -1.0)
  elseif keyboard.isPressed("e") then
    settingsChanged = true

    audioMusic.pitch = math.min(audioMusic.pitch + 0.1, 10.0)
  elseif keyboard.isPressed("d") then
    settingsChanged = true

    audioMusic.pitch = math.max(audioMusic.pitch - 0.1, 0.0)
  elseif keyboard.isPressed("l") then
    audioMusic.looping = not audioMusic.looping

    settingsChanged = true
  end

  if settingsChanged then
    settingsChanged = false

    local isLooping = "N"

    if audioMusic.looping then
      isLooping = "Y"
    end

    settingsText.text = "Volume:  " .. string.format("%.1f", audioMusic.volume) ..
                        "\nPan:    " .. string.format("% .1f", audioMusic.pan) ..
                        "\nPitch:   " .. string.format("%.1f", audioMusic.pitch) ..
                        "\nLooping: " .. isLooping
  end

  if audioMusic.playing then
    cursorText.text = "Playing: " .. math.floor(audioMusic.cursor) .. "/" .. math.floor(audioMusic.length)
  elseif audioMusic.paused then
    cursorText.text = "Paused: " .. math.floor(audioMusic.cursor) .. "/" .. math.floor(audioMusic.length)
  else
    cursorText.text = "Stopped"
  end
end