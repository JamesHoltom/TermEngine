#include "Audio.h"
#include "../logging/Logger.h"
#include "../system/FileFunctions.h"
#include "../utility/AudioUtils.h"

namespace term_engine::resources {
  Audio::Audio(const std::string& filepath, const unsigned int& flag) :
    filepath_(filepath),
    is_paused_(false)
  {
    if (ma_sound_init_from_file(&audio::engine, filepath.c_str(), flag, nullptr, nullptr, &sound_) != MA_SUCCESS)
    {
      logging::logger->error("Failed to load sound \'{}\'.", filepath);
    }
  }

  Audio::~Audio()
  {
    ma_sound_uninit(&sound_);
  }

  void Audio::Play()
  {
    ma_sound_seek_to_pcm_frame(&sound_, 0);
    ma_sound_start(&sound_);
    is_paused_ = false;
  }

  void Audio::Stop()
  {
    ma_sound_stop(&sound_);
    ma_sound_seek_to_pcm_frame(&sound_, 0);
    is_paused_ = false;
  }

  void Audio::Pause()
  {
    ma_sound_stop(&sound_);
    is_paused_ = true;
  }

  void Audio::Resume()
  {
    ma_sound_start(&sound_);
    is_paused_ = false;
  }

  void Audio::SetLooping(const bool& flag)
  {
    ma_sound_set_looping(&sound_, flag ? MA_TRUE : MA_FALSE);
  }

  void Audio::SetPan(const double& pan)
  {
    ma_sound_set_pan(&sound_, (float)pan);
  }

  void Audio::SetPitch(const double& pitch)
  {
    ma_sound_set_pitch(&sound_, (float)pitch);
  }

  void Audio::SetPosition(const glm::vec2& position)
  {
    ma_sound_set_position(&sound_, position.x, 0.0f, position.y);
  }
  
  void Audio::SetVolume(const double& volume)
  {
    ma_sound_set_volume(&sound_, (float)volume);
  }

  bool Audio::IsPlaying() const
  {
    return ma_sound_is_playing(&sound_) == MA_TRUE;
  }

  bool Audio::IsPaused() const
  {
    return is_paused_;
  }

  bool Audio::IsLooping() const
  {
    return ma_sound_is_looping(&sound_) == MA_TRUE;
  }

  double Audio::GetPan() const
  {
    return ma_sound_get_pan(&sound_);
  }

  double Audio::GetPitch() const
  {
    return ma_sound_get_pitch(&sound_);
  }

  glm::vec2 Audio::GetPosition() const
  {
    const ma_vec3f position = ma_sound_get_position(&sound_);

    return glm::vec2(position.x, position.z);
  }

  double Audio::GetVolume() const
  {
    return ma_sound_get_volume(&sound_);
  }

  double Audio::GetLengthSeconds() const
  {
    float length = 0;

    /** @todo
    if (ma_sound_get_length_in_seconds(&sound_, &length) != MA_SUCCESS)
    {
      logging::logger->warn("Failed to get length of audio resource \'{}\'.", filepath_);
    }
    */

    return length;
  }

  std::string Audio::GetFilePath() const
  {
    return filepath_;
  }

  void Audio::PlaySound(const std::string& filepath)
  {
    const std::filesystem::path fullPath = system::SearchForResourcePath(filepath);

    ma_engine_play_sound(&audio::engine, fullPath.c_str(), nullptr);
  }

  AudioPtr& Audio::Add(const std::string& filepath, const std::string& type)
  {
    unsigned int flag = 0;

    if (type == "stream")
    {
      flag = MA_SOUND_FLAG_STREAM;
    }
    else if (type != "static")
    {
      logging::logger->warn("Unknown audio flag \'{}\' given. Defaulting to \'static\'.", type);
    }

    const std::filesystem::path id = GenerateId(filepath);

    audio_list_.emplace(std::make_pair(id, std::make_shared<Audio>(id, flag)));

    return audio_list_.at(id);
  }


  void Audio::Remove(const AudioPtr& resource)
  {
    audio_list_.erase(resource->filepath_);
  }

  bool Audio::Exists(const std::string& filepath)
  {
    const std::filesystem::path id = GenerateId(filepath);

    return audio_list_.find(id) != audio_list_.end();
  }

  AudioMap& Audio::GetList()
  {
    return audio_list_;
  }

  int Audio::Count()
  {
    return audio_list_.size();
  }

  void Audio::CleanUp()
  {
    audio_list_.clear();
  }

  std::string Audio::GenerateId(const std::string& filepath)
  {
    return system::SearchForResourcePath(filepath);
  }

  AudioMap Audio::audio_list_;
}