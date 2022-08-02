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
    Audio(const std::string& filepath, const unsigned int& flag);

    ~Audio();

    void Play();

    void Stop();

    void Pause();

    void Resume();

    void SetLooping(const ma_bool32& flag);

    void SetPan(const double& pan);

    void SetPitch(const double& pitch);

    void SetPosition(const glm::vec2& position);

    void SetVolume(const double& volume);

    bool IsPlaying() const;

    bool IsPaused() const;

    bool IsLooping() const;

    double GetPan() const;

    double GetPitch() const;

    glm::vec2 GetPosition() const;

    double GetVolume() const;

    float GetLengthSeconds() const;

    std::string GetFilePath() const;

    static void PlaySound(const std::string& filepath);

    static AudioPtr& Add(const std::string& filepath, const std::string& type);

    static void Remove(const AudioPtr& audio);

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
    static std::string GenerateId(const std::string& filepath);

    std::filesystem::path filepath_;

    ma_sound sound_;
    bool is_paused_;

    static AudioMap audio_list_;
  };
}

#endif // ! AUDIO_H