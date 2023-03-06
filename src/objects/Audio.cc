#include "Audio.h"
#include "../system/FileFunctions.h"
#include "../utility/AudioUtils.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::objects {
  Audio::Audio(const std::filesystem::path& filepath, const unsigned int& flag) :
    BaseObject(),
    filepath_(filepath),
    is_paused_(false),
    pan_(0.0f),
    pitch_(1.0f),
    position_(glm::vec2(0.0f)),
    volume_(1.0f)
  {
    debug_info_ = utility::AddDebugInfo(GetObjectType() + " " + std::to_string(GetObjectId()));
    
    utility::logger->debug("Created audio resource with ID {} at {}.", object_id_, filepath.string());

    if (ma_sound_init_from_file(&utility::engine, filepath.c_str(), flag, nullptr, nullptr, &sound_) != MA_SUCCESS)
    {
      utility::logger->error("Failed to load sound \'{}\'.", filepath);
    }
  }

  Audio::~Audio()
  {
    ma_sound_uninit(&sound_);

    utility::RemoveDebugInfo(debug_info_);

    utility::logger->debug("Destroyed audio resource with ID {}.", object_id_);
  }

  void Audio::Update()
  {}

  std::string Audio::GetObjectType() const
  {
    return std::string(AUDIO_TYPE);
  }

  ObjectSortPriority Audio::GetListPriority() const
  {
    return ObjectSortPriority::AUDIO;
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
    return pan_;
  }

  double Audio::GetPitch() const
  {
    return pitch_;
  }

  glm::vec2 Audio::GetPosition() const
  {
    return position_;
  }

  double Audio::GetVolume() const
  {
    return volume_;
  }

  double Audio::GetCursorSeconds() const
  {
    float cursor = 0.0f;

    if (ma_data_source_get_cursor_in_seconds(sound_.pDataSource, &cursor) != MA_SUCCESS)
    {
      utility::logger->warn("Failed to get cursor of audio resource \'{}\'.", filepath_);
    }

    return (double)cursor;
  }

  double Audio::GetLengthSeconds() const
  {
    float length = 0.0f;

    if (ma_data_source_get_length_in_seconds(sound_.pDataSource, &length) != MA_SUCCESS)
    {
      utility::logger->warn("Failed to get length of audio resource \'{}\'.", filepath_);
    }

    return (double)length;
  }

  std::string Audio::GetFilePath() const
  {
    return filepath_;
  }

  void Audio::SetLooping(const bool& flag)
  {
    ma_sound_set_looping(&sound_, flag ? MA_TRUE : MA_FALSE);
  }

  void Audio::SetPan(const double& pan)
  {
    pan_ = pan;

    ma_sound_set_pan(&sound_, (float)pan);
  }

  void Audio::SetPitch(const double& pitch)
  {
    pitch_ = pitch;

    ma_sound_set_pitch(&sound_, (float)pitch);
  }

  void Audio::SetPosition(const glm::vec2& position)
  {
    position_ = position;

    ma_sound_set_position(&sound_, position.x, 0.0f, position.y);
  }
  
  void Audio::SetVolume(const double& volume)
  {
    volume_ = volume;
    
    ma_sound_set_volume(&sound_, (float)volume);
  }

  AudioProxy::AudioProxy(const ObjectPtr& object) :
    BaseProxy(object)
  {}

  AudioProxy::~AudioProxy()
  {}

  AudioProxy AddAudio(const std::string& filepath, const std::string& type)
  {
    unsigned int flag = 0;

    if (type == "stream")
    {
      flag = MA_SOUND_FLAG_STREAM;
    }
    else if (type != "static")
    {
      utility::logger->warn("Unknown audio flag \'{}\' given. Defaulting to \'static\'.", type);
    }

    const std::filesystem::path id = system::SearchForResourcePath(filepath);

    is_object_list_dirty_ = true;

    return AudioProxy(object_list_.emplace_front(std::make_shared<Audio>(id, flag)));
  }

  void ClearAllAudio()
  {
    object_list_.remove_if([](const ObjectPtr& object) { return object->GetObjectType() == std::string(AUDIO_TYPE); });

    utility::logger->debug("Cleared all audio objects from the list.");
  }
}