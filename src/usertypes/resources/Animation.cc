#include <optional>
#include "Animation.h"
#include "../../utility/ImGuiUtils.h"

namespace term_engine::usertypes {
  Animation::Animation(const std::string& name) :
    BaseResource(name),
    frames_()
  {
    utility::logger->debug("Created animation resource with name {}.", name_);
  };

  Animation::~Animation()
  {
    utility::logger->debug("Destroyed animation resource with name {}.", name_);
  }

  std::string Animation::GetResourceType() const
  {
    return std::string(ANIMATION_TYPE);
  }

  bool Animation::HasFrames() const
  {
    return !frames_.empty();
  }

  AnimationFrame& Animation::GetFrame(uint32_t index)
  {
    return frames_.at(index);
  }

  uint32_t Animation::GetFrameCount() const
  {
    return frames_.size();
  }

  void Animation::PushFrame(const AnimationFrame& frame)
  {
    frames_.push_back(frame);
  }

  void Animation::AddFrame(const AnimationFrame& frame, uint32_t index)
  {
    if (index < frames_.size())
    {
      AnimationFrameList::const_iterator it = frames_.begin() + index;
      frames_.insert(it, frame);
    }
  }

  void Animation::RemoveFrame(uint32_t index)
  {
    if (index < frames_.size())
    {
      frames_.erase(frames_.begin() + index);
    }
  }

  void Animation::UpdateDebugInfo() const
  {
    if (ImGui::TreeNode((void*)this, "%s (%s)", GetResourceType().c_str(), name_.c_str()))
    {
      ImGui::Text("Name: %s", name_.c_str());

      ImGui::SeparatorText("Frames");

      ImGui::Text("Count: %li", frames_.size());

      ImGui::TreePop();
    }
  }

  AnimationState::AnimationState() :
    animations_(),
    is_playing_(false),
    is_looping_(false),
    is_yoyoing_(false),
    has_queue_changed_(false),
    is_reversing_(false),
    end_of_animation_(false),
    animation_accumulator_(0),
    current_animation_frame_(0),
    frame_rate_(0) {};
  
  void AnimationState::Update(uint32_t timestep)
  {
    if (animations_.empty())
    {
      return;
    }

    if (has_queue_changed_)
    {
      frame_rate_ = animations_.front().delay_;
      is_looping_ = animations_.front().loop_;
      is_yoyoing_ = animations_.front().yoyo_;
      
      has_queue_changed_ = false;
    }

    animation_accumulator_ += timestep;

    uint32_t accumulator_limit = frame_rate_ + animations_.front().animation_->GetFrame(current_animation_frame_).added_duration_;
    int32_t frame_offset = is_reversing_ ? -1 : 1;
    uint32_t last_frame = animations_.front().animation_->GetFrameCount() - 1;

    while (animation_accumulator_ >= accumulator_limit)
    {
      animation_accumulator_ -= accumulator_limit;

      if (end_of_animation_)
      {
        if (is_yoyoing_)
        {
          is_reversing_ = !is_reversing_;
        }
        else if (is_looping_)
        {
          if (is_reversing_)
          {
            current_animation_frame_ = last_frame;
          }
          else
          {
            current_animation_frame_ = 0;
          }
        }
        else
        {
          animations_.pop();

          if (animations_.empty())
          {
            break;
          }
        }

        end_of_animation_ = false;
      }
      else
      {
        current_animation_frame_ += frame_offset;

        if (current_animation_frame_ == last_frame || current_animation_frame_ == 0)
        {
          end_of_animation_ = true;
        }
      }
    }
  }

  AnimationQueue& AnimationState::GetQueue()
  {
    return animations_;
  }

  bool AnimationState::HasAnimationsQueued() const
  {
    return !animations_.empty();
  }

  bool AnimationState::IsPlaying() const
  {
    return is_playing_;
  }

  bool AnimationState::IsLooping() const
  {
    return is_looping_;
  }

  bool AnimationState::IsYoyoing() const
  {
    return is_yoyoing_;
  }

  bool AnimationState::IsReversing() const
  {
    return is_reversing_;
  }

  uint32_t AnimationState::GetFrameRate() const
  {
    return frame_rate_;
  }

  Animation* AnimationState::GetCurrentAnimation()
  {
    return animations_.front().animation_;
  }

  AnimationFrame* AnimationState::GetCurrentFrame()
  {
    if (!animations_.empty())
    {
      return &animations_.front().animation_->GetFrame(current_animation_frame_);
    }
    else
    {
      return nullptr;
    }
  }

  uint32_t AnimationState::GetFrameNumber() const
  {
    return current_animation_frame_;
  }

  void AnimationState::Play()
  {
    is_playing_ = true;
  }

  void AnimationState::Stop()
  {
    is_playing_ = false;
  }

  void AnimationState::Seek(uint32_t frame)
  {
    const uint32_t end_frame = animations_.front().animation_->GetFrameCount() - 1;
    current_animation_frame_ = std::max(frame, end_frame);
  }

  void AnimationState::SetLooping(bool flag)
  {
    is_looping_ = flag;
  }

  void AnimationState::SetYoyoing(bool flag)
  {
    is_yoyoing_ = flag;
  }

  void AnimationState::SetReversing(bool flag)
  {
    is_reversing_ = flag;
  }

  void AnimationState::SetFrameRate(uint32_t frame_rate)
  {
    frame_rate_ = frame_rate;
  }

  void AnimationState::AddToQueue(sol::variadic_args items)
  {
    for (auto item : items)
    {
      Animation* animation = nullptr;
      bool loop = false;
      bool yoyo = false;
      uint32_t frame_rate = DEFAULT_ANIMATION_FRAME_RATE;

      if (item.get_type() == sol::type::string)
      {
        animation = LoadAnimation(item.as<std::string>());
      }
      else if (item.is<Animation*>())
      {
        animation = item.as<Animation*>();
      }
      else if (item.get_type() == sol::type::table)
      {
        sol::table itemTbl = item.as<sol::table>();

        animation = itemTbl.get_or<Animation*>("animation", nullptr);

        if (animation == nullptr)
        {
          std::string name = itemTbl.get_or("name", std::string());
          animation = LoadAnimation(name);
        }

        loop = itemTbl.get_or("loop", false);
        yoyo = itemTbl.get_or("yoyo", false);
        frame_rate = itemTbl.get_or<uint32_t>("frame_rate", DEFAULT_ANIMATION_FRAME_RATE);
      }

      if (animation != nullptr && animation->HasFrames())
      {
        animations_.emplace(animation, loop, yoyo, frame_rate);
      }
    }

    has_queue_changed_ = true;
  }

  void AnimationState::ClearQueue()
  {
    is_playing_ = false;
    current_animation_frame_ = 0;
    is_looping_ = false;
    is_yoyoing_ = false;
    is_reversing_ = false;

    while (!animations_.empty())
    {
      animations_.pop();
    }
  }

  void AnimationState::UpdateDebugInfo() const
  {
    ImGui::SeparatorText("Animation");

    ImGui::Text("Frame: %i/%i", current_animation_frame_, animations_.empty() ? 0 : animations_.front().animation_->GetFrameCount());
    ImGui::Text("Playing?: %s", is_playing_ ? "Yes" : "No");
    ImGui::Text("Looping?: %s", is_looping_ ? "Yes" : "No");
    ImGui::Text("Yoyo-ing?: %s", is_yoyoing_ ? "Yes" : "No");
    ImGui::Text("Reversing?: %s", is_reversing_ ? "Yes" : "No");
    ImGui::Text("Frame Rate: %ums", frame_rate_ + 1);
  }

  Animation* LoadAnimation(const std::string& name)
  {
    if (name.empty())
    {
      return nullptr;
    }

    ResourceList::iterator it = resource_list.find(name);

    if (it == resource_list.end())
    {
      it = resource_list.emplace(std::make_pair(name, std::make_unique<Animation>(name))).first;
    }

    return static_cast<Animation*>(it->second.get());
  }

  void ImportAnimationsFromFile(const std::string& filepath)
  {

  }
}
