/// @author James Holtom

#ifndef AUDIO_H
#define AUDIO_H

#include <filesystem>
#include <string>
#include <glm/glm.hpp>
#include "BaseResource.h"
#include "../../vendor/miniaudio.h"

namespace term_engine::usertypes {
  /// @brief The type name for Audios.
  constexpr char AUDIO_TYPE[] = "Audio";

  /// @brief The master volume for the audio engine.
  inline float master_volume = 1.0f;

  /// @brief Stores an audio resource, used to play music/sounds.
  class Audio : public BaseResource {
  public:
    /**
     * @brief Constructs the resource with the given parameters.
     * 
     * @param[in] filepath  The filepath to the resource.
     * @param[in] sound     The audio data for the resource.
     */
    Audio(const std::filesystem::path& filepath, ma_sound* sound);

    /// @brief Destroys the resource.
    ~Audio();

    /**
     * @brief Returns the type of resource.
     * 
     * @returns The resource type.
     */
    std::string GetResourceType() const;

    /// @brief Plays the audio resource.
    void Play();

    /// @brief Stops the audio resource, and rewinds it back to the start of the audio.
    void Stop();

    /// @brief Pauses the audio resource.
    void Pause();

    /// @brief Rewinds the audio resource to it's starting position.
    void Rewind();

    /**
     * @brief Seeks to the given position in the audio resource.
     * 
     * @param[in] time The point to move the audio cursor to, in milliseconds (ms).
     */
    void Seek(uint64_t time);

    /**
     * @brief Returns if the audio resource is currently playing.
     * 
     * @returns If the audio resource is playing.
     */
    bool IsPlaying() const;

    /**
     * @brief Returns if the audio resource is currently paused.
     * 
     * @returns If the audio resource is paused.
     */
    bool IsPaused() const;

    /**
     * @brief Returns if the audio resource is set to loop playback.
     * 
     * @returns If the audio resource is looping.
     */
    bool IsLooping() const;

    /**
     * @brief Returns the pan for the audio resource.
     * 
     * @returns The pan value.
     */
    double GetPan() const;

    /**
     * @brief Returns the pitch for the audio resource.
     * 
     * @returns The pitch value.
     */
    double GetPitch() const;

    /**
     * @brief Returns the position of the audio resource.
     * 
     * @returns The position of the audio.
     */
    glm::vec2& GetPosition();

    /**
     * @brief Returns the audio volume.
     * 
     * @returns The audio volume.
     */
    double GetVolume() const;

    /**
     * @brief Returns the cursor (i.e. the current position) of the audio resource.
     * 
     * @returns The audio cursor, in seconds.
     */
    double GetCursor() const;

    /**
     * @brief Returns the length of the audio resource.
     * 
     * @returns The audio length, in seconds.
     */
    double GetLength() const;

    /**
     * @brief Sets if the audio resource will loop playback when the audio ends.
     * 
     * @param[in] flag Flag to enable/disable audio looping.
     */
    void SetLooping(bool flag);

    /**
     * @brief Sets the panning between each audio channel.
     * 
     * @param[in] pan The amount to pan the audio by. Accepts a value in between -1.0 and 1.0, and defaults at 0.0.
     */
    void SetPan(double pan);

    /**
     * @brief Sets the pitch of the audio.
     * 
     * @param[in] pitch The amount of pitch. Accepts any value above 0.0, and defaults at 1.0.
     */
    void SetPitch(double pitch);

    /**
     * @brief Sets the position of the audio, as if it exists in a 2D space.
     * 
     * @param[in] position The new position of the audio source.
     */
    void SetPosition(const glm::vec2& position);

    /**
     * @brief Sets the audio volume.
     * 
     * @param[in] volume The new volume. Accepts any value above 0.0 and 1.0, with higher values amplifying the volume.
     */
    void SetVolume(double volume);

    /// @brief Updates the debugging information for this resource.
    void UpdateDebugInfo() const;

  protected:
    /// @brief The audio data.
    ma_sound* sound_;
    /// @brief Is the audio resource currently paused?
    bool is_paused_;
    /// @brief The amount of audio pan (i.e. how the sound is distributed between speakers in a stereo setting).
    double pan_;
    /// @brief The pitch of the audio.
    double pitch_;
    /// @brief The position of the audio (i.e. where in 2D space the sound is playing from).
    glm::vec2 position_;
    /// @brief The audio volume.
    double volume_;
  };

  /**
   * @brief Plays the sound at the given filepath directly.
   * @details Sounds played with function cannot be paused/stopped, nor have their properties updated.
   * 
   * @param[in] filepath The filepath to the sound resource.
   */
  void PlaySound(const std::string& filepath);

  /// @brief Stops all audio resources from playing.
  void StopAllSounds();

  /**
   * @brief Returns the master volume for the audio engine.
   * 
   * @returns The master volume.
   */
  float GetMasterVolume();

  /**
   * @brief Sets the master volume for the audio engine.
   * 
   * @param[in] volume The new value for the master volume.
   */
  void SetMasterVolume(float volume);

  /**
   * @brief Retrives the audio resource with the given filepath. If it's not in the list, it will be loaded.
   * 
   * @param[in] filepath The filepath to the audio resource.
   * @param[in] type     Set to "static" to load audio directly into memory, or "stream" to stream a portion of audio at a time.
   * @remarks Streaming audio with the "stream" type is recommended for large music files. Loading entire audio resources with the "static" type is recommended for small sound files.
   * @returns A raw pointer to the resource, or a null pointer if not found.
   */
  Audio* LoadAudio(const std::string& filepath, const std::string& type);
}

#endif // ! AUDIO_H