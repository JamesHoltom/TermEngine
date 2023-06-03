#include "Audio.h"
#include "../../system/FileFunctions.h"
#include "../../utility/AudioUtils.h"
#include "../../utility/ImGuiUtils.h"
#include "../../utility/SpdlogUtils.h"

namespace term_engine::usertypes {
  Audio::Audio(const std::filesystem::path& filepath, ma_sound* sound) :
    BaseResource(filepath.string()),
    sound_(sound),
    is_paused_(false),
    pan_(0.0f),
    pitch_(1.0f),
    position_(glm::vec2(0.0f)),
    volume_(1.0f)
  {
    utility::logger->debug("Loaded audio resource with filepath {}.", name_);
  }

  Audio::~Audio()
  {
    ma_sound_uninit(sound_);

    utility::logger->debug("Destroyed audio resource with filepath {}.", name_);
  }

  std::string Audio::GetResourceType() const
  {
    return std::string(AUDIO_TYPE);
  }

  void Audio::Play()
  {
    utility::MALog(ma_sound_start(sound_));
    is_paused_ = false;
  }

  void Audio::Stop()
  {
    utility::MALog(ma_sound_stop(sound_));
    utility::MALog(ma_sound_seek_to_pcm_frame(sound_, 0));
    is_paused_ = false;
  }

  void Audio::Pause()
  {
    utility::MALog(ma_sound_stop(sound_));
    is_paused_ = true;
  }

  void Audio::Rewind()
  {
    utility::MALog(ma_sound_seek_to_pcm_frame(sound_, 0));
  }

  void Audio::Seek(uint64_t time)
  {
    const uint32_t sample_rate = ma_engine_get_sample_rate(&utility::audio_engine);
    const ma_uint64 frame = time * (sample_rate / 1000);

    utility::MALog(ma_sound_seek_to_pcm_frame(sound_, frame));
  }

  bool Audio::IsPlaying() const
  {
    return ma_sound_is_playing(sound_) == MA_TRUE;
  }

  bool Audio::IsPaused() const
  {
    return is_paused_;
  }

  bool Audio::IsLooping() const
  {
    return ma_sound_is_looping(sound_) == MA_TRUE;
  }

  double Audio::GetPan() const
  {
    return pan_;
  }

  double Audio::GetPitch() const
  {
    return pitch_;
  }

  glm::vec2& Audio::GetPosition()
  {
    return position_;
  }

  double Audio::GetVolume() const
  {
    return volume_;
  }

  double Audio::GetCursor() const
  {
    float cursor = 0.0f;

    if (utility::MALog(ma_data_source_get_cursor_in_seconds(sound_->pDataSource, &cursor)) != MA_SUCCESS)
    {
      utility::logger->warn("Failed to get cursor of audio resource \'{}\'.", name_);
    }

    return (double)cursor;
  }

  double Audio::GetLength() const
  {
    float length = 0.0f;

    if (utility::MALog(ma_data_source_get_length_in_seconds(sound_->pDataSource, &length)) != MA_SUCCESS)
    {
      utility::logger->warn("Failed to get length of audio resource \'{}\'.", name_);
    }

    return (double)length;
  }

  void Audio::SetLooping(bool flag)
  {
    ma_sound_set_looping(sound_, flag ? MA_TRUE : MA_FALSE);
  }

  void Audio::SetPan(double pan)
  {
    pan_ = pan;

    ma_sound_set_pan(sound_, (float)pan);
  }

  void Audio::SetPitch(double pitch)
  {
    pitch_ = pitch;

    ma_sound_set_pitch(sound_, (float)pitch);
  }

  void Audio::SetPosition(const glm::vec2& position)
  {
    position_ = position;

    ma_sound_set_position(sound_, position.x, 0.0f, position.y);
  }
  
  void Audio::SetVolume(double volume)
  {
    volume_ = volume;
    
    ma_sound_set_volume(sound_, (float)volume);
  }

  void Audio::UpdateDebugInfo() const
  {
    if (ImGui::TreeNode((void*)this, "%s (%s)", GetResourceType().c_str(), name_.c_str()))
    {
      ImGui::Text("Filepath: %s", name_.c_str());
      ImGui::Text("Playing?: %s", IsPlaying() ? "Yes" : "No");
      ImGui::Text("Paused?: %s", is_paused_ ? "Yes" : "No");
      ImGui::Text("Looping?: %s", IsLooping() ? "Yes" : "No");
      ImGui::Text("Pan: %f", GetPan());
      ImGui::Text("Pitch: %f", GetPitch());
      ImGui::Text("Position: %f, %f", position_.x, position_.y);
      ImGui::Text("Volume: %f", GetVolume());
      ImGui::ProgressBar(GetCursor() / GetLength());
      
      ImGui::TreePop();
    }
  }

  Audio* LoadAudio(const std::string& filepath, const std::string& type)
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

    const std::filesystem::path find_path = system::SearchForResourcePath(filepath);

    if (find_path == "")
    {
      utility::logger->warn("Failed to find audio resource at \"{}\".", find_path.string());

      return nullptr;
    }

    ResourceList::iterator it = resource_list.find(find_path.string());

    if (it == resource_list.end())
    {
      ma_sound* new_sound = new ma_sound();

      if (utility::MALog(ma_sound_init_from_file(&utility::audio_engine, find_path.string().c_str(), flag, nullptr, nullptr, new_sound)) != MA_SUCCESS)
      {
        utility::logger->error("Failed to load audio resource at \"{}\".", find_path.string());

        return nullptr;
      }

      it = resource_list.emplace(std::make_pair(find_path.string(), std::make_unique<Audio>(find_path.string(), new_sound))).first;
    }

    return static_cast<Audio*>(it->second.get());
  }
}