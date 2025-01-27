#include "AudioUtils.h"
#include "LogUtils.h"
#include "../system/FileFunctions.h"

namespace term_engine::utility {
  ma_result MALog(ma_result result)
  {
    if (result != MA_SUCCESS)
    {
      LogError("MiniAudio error #{}: {}", (int)result, ma_result_description(result));
    }

    return result;
  }

  ma_format ConvertFromSDLFormat(SDL_AudioFormat format)
  {
    switch (format) {
      case AUDIO_F32: return ma_format::ma_format_f32;      break;
      case AUDIO_S16: return ma_format::ma_format_s16;      break;
      case AUDIO_S32: return ma_format::ma_format_s32;      break;
      case AUDIO_U8:  return ma_format::ma_format_u8;       break;
      default:        return ma_format::ma_format_unknown;  break;
    }
  }

  bool InitAudio()
  {
    ma_engine_config config = ma_engine_config_init();
    config.channels = 2;
    config.sampleRate = 48000;
    config.noDevice = true;

    if (MALog(ma_engine_init(&config, &audio_engine)) != MA_SUCCESS)
    {
      LogError("Failed to initialise audio engine.");

      return false;
    }

    SDL_AudioSpec desired;

    desired.callback = data_callback;
    desired.channels = ma_engine_get_channels(&audio_engine);
    desired.format = AUDIO_F32;
    desired.freq = ma_engine_get_sample_rate(&audio_engine);
    desired.samples = 512;
    desired.userdata = NULL;

    audio_device_id = SDL_OpenAudioDevice(nullptr, 0, &desired, &loaded_audio_engine_config, SDL_AUDIO_ALLOW_ANY_CHANGE);

    if (audio_device_id == 0)
    {
      LogError("Failed to open audio device. {}", SDL_GetError());

      return false;
    }

    SDL_PauseAudioDevice(audio_device_id, SDL_FALSE);

    LogDebug("Initialised miniaudio.");

    return true;
  }

  void CleanUpAudio()
  {
    SDL_CloseAudioDevice(audio_device_id);
    ma_engine_uninit(&audio_engine);

    LogDebug("Shut down miniaudio.");
  }

  void data_callback(void*, uint8_t* buffer, int bufferSizeInBytes)
  {
    /* Reading is just a matter of reading straight from the engine. */
    uint32_t bufferSizeInFrames = (uint32_t)bufferSizeInBytes / ma_get_bytes_per_frame(ma_format_f32, ma_engine_get_channels(&audio_engine));
    ma_engine_read_pcm_frames(&audio_engine, buffer, bufferSizeInFrames, NULL);
  }
}
