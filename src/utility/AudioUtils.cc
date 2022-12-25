#include "AudioUtils.h"
#include "SpdlogUtils.h"
#include "../system/FileFunctions.h"

namespace audio {
  SDL_AudioDeviceID device_id;
  ma_engine engine;

  int Init()
  {
    ma_engine_config config = ma_engine_config_init();
    config.channels = 2;
    config.sampleRate = 48000;
    config.noDevice = true;
    ma_result result = ma_engine_init(&config, &engine);

    if (result != MA_SUCCESS)
    {
      logging::logger->error("Failed to initialise audio engine.");

      return 1;
    }

    SDL_AudioSpec desired;
    SDL_AudioSpec actual;

    desired.callback = data_callback;
    desired.channels = ma_engine_get_channels(&engine);
    desired.format = AUDIO_F32;
    desired.freq = ma_engine_get_sample_rate(&engine);
    desired.samples = 512;
    desired.userdata = NULL;

    device_id = SDL_OpenAudioDevice(nullptr, 0, &desired, &actual, SDL_AUDIO_ALLOW_ANY_CHANGE);

    if (device_id == 0)
    {
      logging::logger->error("Failed to open audio device. {}", SDL_GetError());

      return 1;
    }

    SDL_PauseAudioDevice(device_id, SDL_FALSE);

    logging::logger->debug("Initialised miniaudio.");

    return 0;
  }

  void CleanUp()
  {
    SDL_CloseAudioDevice(device_id);
    ma_engine_uninit(&engine);

    logging::logger->debug("Shut down miniaudio.");
  }

  void data_callback(void* userdata, ma_uint8* buffer, int bufferSizeInBytes)
  {
    /* Reading is just a matter of reading straight from the engine. */
    ma_uint32 bufferSizeInFrames = (ma_uint32)bufferSizeInBytes / ma_get_bytes_per_frame(ma_format_f32, ma_engine_get_channels(&engine));
    ma_engine_read_pcm_frames(&engine, buffer, bufferSizeInFrames, NULL);
  }
}
