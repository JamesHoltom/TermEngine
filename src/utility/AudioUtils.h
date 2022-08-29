#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

// Device IO will be handled by SDL2.
#define MA_NO_DEVICE_IO

#include <SDL2/SDL_audio.h>
#include "../vendor/miniaudio.h"

namespace audio {
  /// @brief The ID of the audio device in SDL.
  extern SDL_AudioDeviceID device_id;
  /// @brief Handler to the audio engine.
  extern ma_engine engine;

  /**
   * @brief Initialises the audio engine.
   * 
   * @returns If the engine was successfully initialised.
   */
  int Init();

  /// @brief Destroys the audio engine and does cleanup.
  void CleanUp();

  /**
   * @brief A callback for the audio engine to capture incoming audio and read it to the engine.
   * 
   * @param[in,out] userdata      Unused userdata.
   * @param[out] buffer           The buffer to read audio data to.
   * @param[in] bufferSizeInBytes The size of the buffer.
   */
  void data_callback(void* userdata, ma_uint8* buffer, int bufferSizeInBytes);
}

#endif // ! AUDIO_UTILS_H
