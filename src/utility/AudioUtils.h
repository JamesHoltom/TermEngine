#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

#define MA_NO_DEVICE_IO

#include <SDL2/SDL_audio.h>
#include "../vendor/miniaudio.h"

namespace audio {
  extern SDL_AudioDeviceID device_id;
  extern ma_engine engine;

  int Init();

  void CleanUp();

  void data_callback(void* userdata, ma_uint8* buffer, int bufferSizeInBytes);
}

#endif // ! AUDIO_UTILS_H
