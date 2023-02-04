/// @author James Holtom

#ifndef AUDIO_H
#define AUDIO_H

#include <filesystem>
#include <optional>
#include <string>
#include <glm/glm.hpp>
#include "BaseObject.h"
#include "../vendor/miniaudio.h"

/*
 * TODO: 
 *   - Add "Seek" function.
 *   - Condense "GetCursor" and "GetLength" functions into single functions, that take the format of the cursor/length as a parameter.
 *   - Consider implementing sound groups & effects.
 */

namespace term_engine::objects {
  constexpr char AUDIO_TYPE[] = "Audio";

  /// @brief Stores an audio resource, used to play music/sounds.
  class Audio : public BaseObject {
  public:
    /**
     * @brief Constructs the object with the given parameters.
     * 
     * @param[in] filepath The filepath to the audio resource.
     * @param[in] flag     Used to set if the whole resource is loaded or streamed into memory.
     * @remarks The `flag` parameter uses _0_ to represent a "static" resource, and _ma_sound_flags::MA_SOUND_FLAG_STREAM_ to represent a "stream" resource.
     */
    Audio(const std::filesystem::path& filepath, const unsigned int& flag);

    /// @brief Destroys the object.
    ~Audio();

    /// @brief Updates the object.
    void Update();

    /**
     * @brief Returns the type of object.
     * 
     * @returns The object type.
     */
    std::string GetObjectType() const;

    /**
     * @brief Returns the list priority for this object.
     * @details This is used to sort the list of objects before updating.
     * 
     * @returns The priority for this object.
     */
    ObjectSortPriority GetListPriority() const;

    /// @brief Plays the audio resource at the start of the audio.
    void Play();

    /// @brief Stops the audio resource, and rewinds it back to the start of the audio.
    void Stop();

    /// @brief Pauses the audio resource.
    void Pause();

    /// @brief Re-starts the audio resource, playing from wherever it was paused.
    void Resume();

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
    glm::vec2 GetPosition() const;

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
    double GetCursorSeconds() const;

    /**
     * @brief Returns the length of the audio resource.
     * 
     * @returns The audio length, in seconds.
     */
    double GetLengthSeconds() const;

    /**
     * @brief Returns the filepath the audio resource is located at.
     * 
     * @returns The resource filepath.
     */
    std::string GetFilePath() const;

    /**
     * @brief Sets if the audio resource will loop playback when the audio ends.
     * 
     * @param[in] flag Flag to enable/disable audio looping.
     */
    void SetLooping(const bool& flag);

    /**
     * @brief Sets the panning between each audio channel.
     * 
     * @param[in] pan The amount to pan the audio by. Accepts a value in between -1.0 and 1.0, and defaults at 0.0.
     */
    void SetPan(const double& pan);

    /**
     * @brief Sets the pitch of the audio.
     * 
     * @param[in] pitch The amount of pitch. Accepts any value above 0.0, and defaults at 1.0.
     */
    void SetPitch(const double& pitch);

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
    void SetVolume(const double& volume);

  private:
    /// @brief The filepath to the audio resource.
    std::filesystem::path filepath_;
    /// @brief Handle to the audio data.
    ma_sound sound_;
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

  /// @brief An object proxy to interact with audio resources.
  class AudioProxy : public BaseProxy {
  public:
    /**
     * @brief Constructs the object proxy.
     * 
     * @param[in] object A smart pointer to the object.
     */
    AudioProxy(const ObjectPtr& object);

    /// @brief Destroys the object proxy.
    ~AudioProxy();
    
    OBJECT_PROXY_GETTER(Audio, GetObjectType, std::string)
    OBJECT_PROXY_CALLER(Audio, Play)
    OBJECT_PROXY_CALLER(Audio, Stop)
    OBJECT_PROXY_CALLER(Audio, Pause)
    OBJECT_PROXY_CALLER(Audio, Resume)
    OBJECT_PROXY_GETTER(Audio, IsPlaying, bool)
    OBJECT_PROXY_GETTER(Audio, IsPaused, bool)
    OBJECT_PROXY_GETTER(Audio, IsLooping, bool)
    OBJECT_PROXY_GETTER(Audio, GetPan, double)
    OBJECT_PROXY_GETTER(Audio, GetPitch, double)
    OBJECT_PROXY_GETTER(Audio, GetPosition, glm::vec2)
    OBJECT_PROXY_GETTER(Audio, GetVolume, double)
    OBJECT_PROXY_GETTER(Audio, GetCursorSeconds, double)
    OBJECT_PROXY_GETTER(Audio, GetLengthSeconds, double)
    OBJECT_PROXY_GETTER(Audio, GetFilePath, std::string)
    OBJECT_PROXY_SETTER(Audio, SetLooping, bool)
    OBJECT_PROXY_SETTER(Audio, SetPan, double)
    OBJECT_PROXY_SETTER(Audio, SetPitch, double)
    OBJECT_PROXY_SETTER(Audio, SetPosition, glm::vec2)
    OBJECT_PROXY_SETTER(Audio, SetVolume, double)
  };

  /**
   * @brief Adds an audio object to the list.
   * 
   * @param[in] filepath The filepath to the audio object.
   * @param[in] type     Set to "static" to load audio directly into memory, or "stream" to stream a portion of audio at a time.
   * @remarks Streaming audio with the "stream" type is recommended for large music files. Loading entire audio resources with the "static" type is recommended for small sound files.
   * @returns A proxy to the object.
   */
  AudioProxy AddAudio(const std::string& filepath, const std::string& type);

  /// @brief Clears all audio objects from the object list.
  void ClearAllAudio();
}

#endif // ! AUDIO_H