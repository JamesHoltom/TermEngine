--[[
    This project uses audio from OpenGameArt.org, and are distributed under the CC0 license.

    - 8bit SFX, by celestialghost8: https://opengameart.org/content/8bit-sfx
    - NES Shooter Music, by SketchyLogic: https://opengameart.org/content/nes-shooter-music-5-tracks-3-jingles

    CC0 license: https://creativecommons.org/publicdomain/zero/1.0/
  ]]

local audioText, cursorText, settingsText, audioMusic, audioSeek, audioLength

function Init()
  defaultGameScene.characterMap.size = Ivec2(42, 7)
  defaultWindow:resizeToCharacterMap()

  audioMusic = Audio("audio/Map.wav", "stream")

  audioSeek = 1000
  audioLength = audioMusic.length * 1000

  audioText = TextObject(Values.IVEC2_ZERO, Ivec2(26, 6), "Press 'n' to play sound" ..
  "\nPress 'm' to play music" ..
  "\nPress 'p' to pause" ..
  "\nPress 'l' to loop" ..
  "\nPress 'r' to rewind" ..
  "\nPress 't' to seek to time")
  cursorText = TextObject(Ivec2(0, 6), Ivec2(28, 1), "Stopped")
  settingsText = TextObject(Ivec2(28, 0), Ivec2(14, 5), "Volume:  " .. string.format("%.1f", audioMusic.volume) ..
  "\nPan:    " .. string.format("% .1f", audioMusic.pan) ..
  "\nPitch:   " .. string.format("%.1f", audioMusic.pitch) ..
  "\nLooping: N" ..
  "\nSeek:    " .. audioSeek)

  return true
end

function Loop(timestep)
  local settingsChanged = false

  if keyboard.isPressed("n") then
    audio.play("audio/1.wav")
  elseif keyboard.isPressed("m") then
    if audioMusic.playing then
      print("Stop")
      audioMusic:stop()
      cursorText.text = "Stopped"
    else
      print("Play")
      audioMusic:play()
    end
  elseif keyboard.isPressed("p") then
    if audioMusic.paused then
      print("Resume")
      audioMusic:play()
    elseif audioMusic.playing then
      print("Pause")
      audioMusic:pause()
      cursorText.text = "Paused: " .. math.floor(audioMusic.cursor) .. "/" .. math.floor(audioMusic.length)
    end
  elseif keyboard.isPressed("t") then
    print("Seek")
    audioMusic:seek(audioSeek)
  elseif keyboard.isPressed("r") then
    print("Rewind")
    audioMusic:rewind()
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
  elseif keyboard.isPressed("[") then
    audioSeek = audioSeek - 100

    if audioSeek < 0 then
      audioSeek = 0
    end

    settingsChanged = true
  elseif keyboard.isPressed("]") then
    audioSeek = audioSeek + 100

    if audioSeek > audioLength then
      audioSeek = audioLength
    end

    settingsChanged = true
  end

  if settingsChanged == true then
    settingsChanged = false

    local isLooping = "N"

    if audioMusic.looping then
      isLooping = "Y"
    end

    settingsText.text = "Volume:  " .. string.format("%.1f", audioMusic.volume) ..
                        "\nPan:    " .. string.format("% .1f", audioMusic.pan) ..
                        "\nPitch:   " .. string.format("%.1f", audioMusic.pitch) ..
                        "\nLooping: " .. isLooping ..
                        "\nSeek:    " .. audioSeek
  end

  if audioMusic.playing then
    cursorText.text = "Playing: " .. math.floor(audioMusic.cursor) .. "/" .. math.floor(audioMusic.length)
  end
end