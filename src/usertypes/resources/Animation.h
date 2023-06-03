/// @author James Holtom

#ifndef ANIMATION_H
#define ANIMATION_H

#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
#include "BaseResource.h"
#include "../CharacterMap.h"
#include "../../utility/GLUtils.h"
#include "../../utility/SolUtils.h"

namespace term_engine::usertypes {
  struct AnimationFrame;
  struct AnimationQueueItem;
  class Animation;

  typedef std::vector<AnimationFrame> AnimationFrameList;
  typedef std::queue<AnimationQueueItem> AnimationQueue;

  /// @brief The type name for Animation.
  constexpr char ANIMATION_TYPE[] = "Animation";

  constexpr uint32_t DEFAULT_ANIMATION_FRAME_RATE = 1000 / 60;

  struct AnimationFrame {
    CharacterData data_;
    glm::ivec2 size_;
    glm::ivec2 offset_;
    uint32_t added_duration_;

    AnimationFrame() :
      data_(),
      size_(glm::ivec2()),
      offset_(glm::ivec2()),
      added_duration_(0) {}

    AnimationFrame(const sol::table& data, const glm::ivec2& size, const glm::ivec2& offset, uint32_t duration) :
      size_(size),
      offset_(offset),
      added_duration_(duration)
    {
        for (auto item : data)
        {
          if (item.second.is<CharacterParams>())
          {
            data_.push_back(item.second.as<CharacterParams>());
          }
        }
    };

    AnimationFrame(const sol::table& data, const glm::ivec2& size, const glm::ivec2& offset) :
      AnimationFrame(data, size, offset, 0) {};

    AnimationFrame(const sol::table& data, const glm::ivec2& size) :
      AnimationFrame(data, size, glm::ivec2(), 0) {};
  };

  struct AnimationQueueItem {
    Animation* animation_;
    bool loop_;
    bool yoyo_;
    uint32_t delay_;

    AnimationQueueItem(Animation* animation, bool loop, bool yoyo, uint32_t delay) :
      animation_(animation),
      loop_(loop),
      yoyo_(yoyo),
      delay_(delay) {};
  };

  class Animation : public BaseResource {
  public:
    /**
     * @brief Constructs the resource with the given name.
     * 
     * @param[in] name  The name to the resource.
     */
    Animation(const std::string& name);

    /// @brief Destroys the resource.
    ~Animation();

    /**
     * @brief Returns the type of resource.
     * 
     * @returns The resource type.
     */
    std::string GetResourceType() const;

    bool HasFrames() const;

    AnimationFrame& GetFrame(uint32_t index);

    uint32_t GetFrameCount() const;

    void PushFrame(const AnimationFrame& frame);

    void AddFrame(const AnimationFrame& frame, uint32_t index);

    void RemoveFrame(uint32_t index);

    /// @brief Updates the debugging information for this animation.
    void UpdateDebugInfo() const;

    /**
     * @brief Allows for comparing 2 sets of _Animation_ objects.
     * 
     * @param[in] lhs The left-hand side object.
     * @param[in] rhs The right-hand side object.
     * @returns If the 2 objects have equal values.
     */
    friend bool operator== (const Animation& lhs, const Animation& rhs)
    {
      return lhs.name_ == rhs.name_;
    }
    
  private:
    AnimationFrameList frames_;
  };

  class AnimationState {
  public:
    AnimationState();

    void Update(uint32_t timestep);

    AnimationQueue& GetQueue();

    bool HasAnimationsQueued() const;

    bool IsPlaying() const;

    bool IsLooping() const;

    bool IsYoyoing() const;

    bool IsReversing() const;

    uint32_t GetFrameRate() const;

    Animation* GetCurrentAnimation();

    AnimationFrame* GetCurrentFrame();

    uint32_t GetFrameNumber() const;

    void Play();

    void Stop();

    void Seek(uint32_t frame);

    void SetLooping(bool flag);

    void SetYoyoing(bool flag);

    void SetReversing(bool flag);

    void SetFrameRate(uint32_t frame_rate);

    void AddToQueue(sol::variadic_args items);

    void ClearQueue();

    /// @brief Updates the debugging information for this animation.
    void UpdateDebugInfo() const;

  private:
    AnimationQueue animations_;

    bool is_playing_;

    bool is_looping_;

    bool is_yoyoing_;

    bool has_queue_changed_;

    bool is_reversing_;

    bool end_of_animation_;

    uint32_t animation_accumulator_;

    uint32_t current_animation_frame_;

    uint32_t frame_rate_;
  };

  Animation* LoadAnimation(const std::string& name);

  void ImportAnimationsFromFile(const std::string& filepath);
}

#endif // ! ANIMATION_H
