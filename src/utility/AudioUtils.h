#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

// Device IO will be handled by SDL2.
#define MA_NO_DEVICE_IO

#include <SDL2/SDL_audio.h>
#include "../vendor/miniaudio.h"

namespace term_engine::utility {
  /// @brief The ID of the audio device in SDL.
  inline uint32_t audio_device_id;
  /// @brief Handler to the audio engine.
  inline ma_engine audio_engine;
  
  inline SDL_AudioSpec loaded_audio_engine_config;

  struct ma_soundDeleter {
    void operator()(ma_sound* ptr)
    {
      ma_sound_uninit(ptr);
    }
  };

  /**
   * @brief Logs any errors that occur when calling a miniaudio function.
   * 
   * @param[in] result The result of the miniaudio call to check.
   * @returns The result that was passed in.
   */
  ma_result MALog(ma_result result);

  /**
   * @brief Converts an SDL-style audio format value to it's miniaudio-style equivalent.
   * 
   * @param[in] format The SDL-style audio format value.
   * @returns The miniaudio-style audio format value.
   */
  ma_format ConvertFromSDLFormat(SDL_AudioFormat format);

  /**
   * @brief Initialises the audio engine.
   * 
   * @returns If the engine was successfully initialised.
   */
  bool InitAudio();

  /// @brief Destroys the audio engine and does cleanup.
  void CleanUpAudio();

  /**
   * @brief A callback for the audio engine to capture incoming audio and read it to the engine.
   * 
   * @param[in,out] userdata          Unused userdata.
   * @param[out]    buffer            The buffer to read audio data to.
   * @param[in]     bufferSizeInBytes The size of the buffer.
   */
  void data_callback(void* userdata, uint8_t* buffer, int bufferSizeInBytes);
}

#endif // ! AUDIO_UTILS_H
