#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

// Device IO will be handled by SDL2.
#define MA_NO_DEVICE_IO

#include <SDL2/SDL_audio.h>
#include "../vendor/miniaudio.h"

namespace audio {
  extern SDL_AudioDeviceID device_id;
  extern ma_engine engine;

  /// Initialises the audio engine.
  /**
   * @returns If the engine was successfully initialised.
   */
  int Init();

  /// Destroys the audio engine and does cleanup.
  void CleanUp();

  /// A callback for the audio engine to capture incoming audio and read it to the engine.
  /**
   * @param[in,out] userdata      Unused userdata.
   * @param[out] buffer           The buffer to read audio data to.
   * @param[in] bufferSizeInBytes The size of the buffer.
   */
  void data_callback(void* userdata, ma_uint8* buffer, int bufferSizeInBytes);
}

#endif // ! AUDIO_UTILS_H
