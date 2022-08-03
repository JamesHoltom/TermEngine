/// @author James Holtom

#ifndef AUDIO_H
#define AUDIO_H

#include <glm/glm.hpp>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include "../vendor/miniaudio.h"

namespace term_engine::resources {
  class Audio;

  typedef std::shared_ptr<Audio> AudioPtr;
  typedef std::unordered_map<std::string, AudioPtr> AudioMap;

  class Audio {
  public:
    /// Constructs the audio resource with the given parameters.
    /**
     * @param[in] filepath The filepath to the audio resource.
     * @param[in] flag     Used to set if the whole resource is loaded or streamed into memory.
     * @remarks The `flag` parameter uses _0_ to represent a "static" resource, and _ma_sound_flags::MA_SOUND_FLAG_STREAM_ to represent a "stream" resource.
     */
    Audio(const std::string& filepath, const unsigned int& flag);

    /// Destroys the audio resource.
    ~Audio();

    /// Plays the audio resource at the start of the audio.
    void Play();

    /// Stops the audio resource, and rewinds it back to the start of the audio.
    void Stop();

    /// Pauses the audio resource.
    void Pause();

    /// Re-starts the audio resource, playing from wherever it was paused.
    void Resume();

    /// Sets if the audio resource will loop playback when the audio ends.
    /**
     * @param[in] flag Flag to enable/disable audio looping.
     */
    void SetLooping(const bool& flag);

    /// Sets the panning between each audio channel.
    /**
     * @param[in] pan The amount to pan the audio by. Accepts a value in between -1.0 and 1.0, and defaults at 0.0.
     */
    void SetPan(const double& pan);

    /// Sets the pitch of the audio.
    /**
     * @param[in] pitch The amount of pitch. Accepts any value above 0.0, and defaults at 1.0.
     */
    void SetPitch(const double& pitch);

    /// Sets the position of the audio, as if it exists in a 2D space.
    /**
     * @param[in] position The new position of the audio source.
     */
    void SetPosition(const glm::vec2& position);

    /// Sets the audio volume.
    /**
     * @param[in] volume The new volume. Accepts a value in between 0.0 and 1.0, and defaults to 1.0.
     */
    void SetVolume(const double& volume);

    /// Returns if the audio resource is currently playing.
    /**
     * @returns If the audio resource is playing.
     */
    bool IsPlaying() const;

    /// Returns if the audio resource is currently paused.
    /**
     * @returns If the audio resource is paused.
     */
    bool IsPaused() const;

    /// Returns if the audio resource is set to loop playback.
    /**
     * @returns If the audio resource is looping.
     */
    bool IsLooping() const;

    /// Returns the pan for the audio resource.
    /**
     * @returns The pan value.
     */
    double GetPan() const;

    /// Returns the pitch for the audio resource.
    /**
     * @returns The pitch value.
     */
    double GetPitch() const;

    /// Returns the position of the audio resource.
    /**
     * @returns The position of the audio.
     */
    glm::vec2 GetPosition() const;

    /// Returns the audio volume.
    /**
     * @returns The audio volume.
     */
    double GetVolume() const;

    /// Returns the length of the audio resource.
    /**
     * @returns The audio length.
     */
    double GetLengthSeconds() const;

    /// Returns the filepath the audio resource is located at.
    /**
     * @returns The resource filepath.
     */
    std::string GetFilePath() const;

    /// Plays an audio file directly through the engine.
    /**
     * @param[in] filepath The filepath to the audio file.
     */
    static void PlaySound(const std::string& filepath);

    /// Adds an audio resource to the list.
    /**
     * @param[in] filepath The filepath to the audio resource.
     * @param[in] type     Set to "static" to load audio directly into memory, or "stream" to stream a portion of audio at a time.
     * @remarks Streaming audio with the "stream" type is recommended for large music files. Loading entire audio resources with the "static" type is recommended for small sound files.
     * @returns A smart pointer to the resource if it was added to the list, or a null pointer if it failed.
     */
    static AudioPtr& Add(const std::string& filepath, const std::string& type);

    /// Removes an audio resource from the list.
    /**
     * @param[in] audio A smart pointer to the resource.
     */
    static void Remove(AudioPtr& audio);

    /// Returns if an audio resource exists with the given filepath.
    /**
     * @param[in] filepath The filepath to check.
     * @returns If an audio resource exists.
     */
    static bool Exists(const std::string& filepath);

    /// Retrieves the list of audio resources.
    /**
     * @returns The list of audio resources.
     */
    static AudioMap& GetList();

    /// Returns the number of audio resources.
    /**
     * @returns The number of audio resources.
     */
    static int Count();
    
    /// Destroys all audio resources in the list.
    static void CleanUp();

  protected:
    /// Generates an ID from the given filepath, to use as the list key.
    /**
     * This is used to prevent any given resource from being loaded multiple times.
     * 
     * @param[in] filepath The filepath to the audio resource.
     * @returns The ID of the audio resource.
     */
    static std::string GenerateId(const std::string& filepath);

    /// The filepath to the audio resource.
    std::filesystem::path filepath_;
    /// Handle to the audio data.
    ma_sound sound_;
    /// Is the audio resource currently paused?
    bool is_paused_;

    /// The list of audio resources.
    static AudioMap audio_list_;
  };
}

#endif // ! AUDIO_H