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

  /// @brief Used to store a list of animation frames within an animation.
  typedef std::vector<AnimationFrame> AnimationFrameList;
  /// @brief Used to queue animations to be rendered to an object.
  typedef std::queue<AnimationQueueItem> AnimationQueue;

  /// @brief The type name for Animation.
  constexpr char ANIMATION_TYPE[] = "Animation";
  /// @brief The default frame rate for animations, set to 24 frames per second (or 113ms).
  constexpr uint32_t DEFAULT_ANIMATION_FRAME_RATE = 24;

  /// @brief Stores a map of characters that represents a frame of animation.
  class AnimationFrame {
  public:
    /// @brief Constructs the animation frame.
    AnimationFrame();

    /**
     * @brief Constructs the animation frame with the given parameters.
     * 
     * @param[in] data The character data for the frame.
     * @param[in] size The size of the frame.
     * @param[in] offset The position of the frame, relative to the game object using it.
     * @param[in] duration The additional delay to add to the frame rate when rendering the frame.
     */
    AnimationFrame(const sol::table& data, const glm::ivec2& size, const glm::ivec2& offset, int32_t duration);

    /**
     * @brief Constructs the animation frame with the given parameters.
     * 
     * @param[in] data The character data for the frame.
     * @param[in] size The size of the frame.
     * @param[in] offset The position of the frame, relative to the game object using it.
     */
    AnimationFrame(const sol::table& data, const glm::ivec2& size, const glm::ivec2& offset);

    /**
     * @brief Constructs the animation frame with the given parameters.
     * 
     * @param[in] data The character data for the frame.
     * @param[in] size The size of the frame.
     */
    AnimationFrame(const sol::table& data, const glm::ivec2& size);

    /**
     * @brief Returns the frame's character data.
     * 
     * @returns The data within the frame.
     */
    CharacterMap& GetCharacterMap();

    /**
     * @brief Returns the position of the frame, relative to the game object using it.
     * 
     * @returns The position of the frame, in rows/columns.
     */
    glm::ivec2& GetOffset();

    /**
     * @brief Returns the additional delay to add to the frame rate when rendering the frame.
     * 
     * @returns The delay, in milliseconds (ms).
     */
    int& GetAddedDuration();

    /**
     * @brief Sets the character data for the frame.
     * 
     * @param[in] data The character data.
     */
    void SetCharacterMap(const CharacterMap& data);

    /**
     * @brief Sets the position of the frame, relative to the game object using it.
     * 
     * @param[in] offset The position of the frame, in rows/columns.
     */
    void SetOffset(const glm::ivec2& offset);

    /**
     * @brief Sets the additional delay to add to the frame rate when rendering the frame.
     * 
     * @param[in] duration The delay, in milliseconds (ms).
     */
    void SetAddedDuration(int duration);

    /// @brief Updates the debugging information for this resource.
    void UpdateDebugInfo();

  private:
    /// @brief The character data to store.
    CharacterMap data_;
    /// @brief The position of the frame, relative to the object.
    glm::ivec2 offset_;
    /// @brief Additional delay added to the frame rate, in milliseconds (ms).
    int added_duration_;
  };

  /// @brief Represents an animation queued into an object.
  struct AnimationQueueItem {
    /// @brief A raw pointer to the animation.
    Animation* animation_;
    /// @brief Should the animation start when queued?
    bool start_;
    /// @brief Should the animation loop?
    bool loop_;
    /// @brief Should the animation yoyo (i.e. alternate between playing forwards/reverse)?
    bool yoyo_;
    /// @brief The frame rate to render the animation at, in milliseconds (ms).
    uint32_t frame_duration_;

    /**
     * @brief Construct a new Animation Queue Item object
     * 
     * @param[in] animation A raw pointer to the animation resource.
     * @param[in] start     Should the animation start when queued?
     * @param[in] loop      Should the animation loop?
     * @param[in] yoyo      Should the animation yoyo?
     * @param[in] delay     The frame rate to render the animation at.
     */
    AnimationQueueItem(Animation* animation, bool start, bool loop, bool yoyo, uint32_t delay) :
      animation_(animation),
      start_(start),
      loop_(loop),
      yoyo_(yoyo),
      frame_duration_(delay) {};
  };

  /// @brief Stores a series of animation frames.
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

    /**
     * @brief Returns if the animation has frames to render.
     * 
     * @returns If the animation has frames to render.
     */
    bool HasFrames() const;

    /**
     * @brief Returns the frame data at the given index.
     * 
     * @param[in] index The index of the frame data to retrieve.
     * @returns The frame data.
     */
    AnimationFrame* GetFrame(uint32_t index);

    /**
     * @brief Returns the number of frames in the animation.
     * 
     * @returns The number of animation frames.
     */
    uint32_t GetFrameCount() const;

    /**
     * @brief Pushes a new frame to the end of the animation.
     * 
     * @param[in] frame The frame data to add.
     */
    void PushFrame(const AnimationFrame& frame);

    /**
     * @brief Adds a new frame to the animation at the given index.
     * 
     * @param[in] frame The frame data to add.
     * @param[in] index The index to insert the frame at.
     */
    void AddFrame(const AnimationFrame& frame, uint32_t index);

    /**
     * @brief Sets the frame data at the given index.
     * 
     * @param[in] frame The new frame data.
     * @param[in] index The index of the frame to set.
     */
    void SetFrame(const AnimationFrame& frame, uint32_t index);

    /**
     * @brief Removes the frame at the given index.
     * 
     * @param[in] index The index of the frame to remove.
     */
    void RemoveFrame(uint32_t index);

    /**
     * @brief Returns the list of animation frames.
     * 
     * @returns The list of animation frames. 
     */
    AnimationFrameList& GetFrames();

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
    /// @brief The list of animation frames.
    AnimationFrameList frames_;
  };

  /// @brief Used to queue and manage animations to be rendered to an object.
  class AnimationState {
  public:
    /// @brief Constructs the animation state.
    AnimationState();

    /**
     * @brief Updates the animation state.
     * 
     * @param[in] timestep The time since the last update, in milliseconds (ms).
     */
    void Update(uint32_t timestep);

    /**
     * @brief Returns the list of queued animations.
     * 
     * @returns The list of animations in the queue.
     */
    AnimationQueue& GetQueue();

    /**
     * @brief Returns if there are animations queued in the state.
     * 
     * @returns If animations are queued in the state.
     */
    bool HasAnimationsQueued() const;

    /**
     * @brief Returns if an animation is currently playing.
     * 
     * @returns If an animation is currently playing.
     */
    bool IsPlaying() const;

    /**
     * @brief Returns if the current animation is set to loop.
     * 
     * @returns If the current animation is looping.
     */
    bool IsLooping() const;

    /**
     * @brief Returns if the current animation is set to yoyo (i.e. alternate between playing forwards/reverse).
     * 
     * @returns If the current animation is yoyo-ing.
     */
    bool IsYoyoing() const;

    /**
     * @brief Returns if the animation is set to play in reverse.
     * 
     * @returns If the animation is playing in reverse.
     */
    bool IsReversing() const;

    /**
     * @brief Returns how long each frame of the animation plays for.
     * 
     * @returns How long each frame plays for, in milliseconds (ms).
     */
    uint32_t GetFrameDuration() const;

    /**
     * @brief Returns the frame rate for playing the current animation.
     * 
     * @returns The frame rate.
     */
    uint32_t GetFrameRate() const;

    /**
     * @brief Returns the currently queued animation.
     * 
     * @returns A raw pointer to the animation, or a null pointer if the queue is empty.
     */
    Animation* GetCurrentAnimation();

    /**
     * @brief Returns the current frame data for the currently queued animation.
     * 
     * @returns A raw pointer to the frame data, or a null pointer if the queue is empty.
     */
    AnimationFrame* GetCurrentFrame();

    /**
     * @brief Returns the current frame index.
     * 
     * @returns The frame index.
     */
    uint32_t GetFrameNumber() const;

    /// @brief Starts playing the currently queued animation.
    void Play();

    /// @brief Stops playing the currently queued animation.
    void Stop();

    /**
     * @brief Seeks to the frame in the currently queued animation, at the given index.
     * 
     * @param[in] frame The index of the frame to move to.
     */
    void Seek(uint32_t frame);

    /**
     * @brief Sets if the currently queued animation will loop.
     * 
     * @param[in] flag Flag to enable/disable looping.
     */
    void SetLooping(bool flag);

    /**
     * @brief Sets if the currently queued animation will yoyo (i.e. alternate between playing forwards/reverse).
     * 
     * @param[in] flag Flag to enable/disable yoyo-ing.
     */
    void SetYoyoing(bool flag);

    /**
     * @brief Sets if the currently queued animation will play in reverse.
     * 
     * @param[in] flag Flag to enable/disable reverse play.
     */
    void SetReversing(bool flag);

    /**
     * @brief Sets how long each frame of the current animation plays for.
     * 
     * @param[in] frame_duration How long each frame plays for, in milliseconds (ms).
     */
    void SetFrameDuration(uint32_t frame_duration);

    /**
     * @brief Sets the frame rate for playing the current animation.
     * 
     * @param[in] frame_rate The frame rate.
     */
    void SetFrameRate(uint32_t frame_rate);

    /**
     * @brief Adds 1 or more animations to the queue.
     * 
     * @param[in] items The list of animations to queue.
     */
    void AddToQueue(const sol::variadic_args& items);

    /// @brief Clears the queue of all animations.
    void ClearQueue();

    /// @brief Updates the debugging information for this animation.
    void UpdateDebugInfo() const;

  private:
    /// @brief The list of queued animations.
    AnimationQueue animations_;
    /// @brief Is an animation playing?
    bool is_playing_;
    /// @brief Is the current animation looping?
    bool is_looping_;
    /// @brief Is the current animation yoyo-ing (i.e. alternate between playing forwards/reverse)?
    bool is_yoyoing_;
    /// @brief Has the queue changed, and the state needs to be updated?
    bool has_queue_changed_;
    /// @brief Is the current animation set to play in reverse?
    bool is_reversing_;
    /// @brief Has the animation reached the end of play?
    bool end_of_animation_;
    /// @brief The amount of time the current frame has been rendered for.
    uint32_t animation_accumulator_;
    /// @brief The index of the current frame of animation.
    uint32_t current_animation_frame_;
    /// @brief How long each frame plays for, in milliseconds (ms).
    uint32_t frame_duration_;
  };

  /**
   * @brief Retrieves the animation resource with the given name. If it's not in the list, it will be created.
   * 
   * @param[in] name The name of the animation resource.
   * @returns A raw pointer to the resource, or a null pointer if given an empty name.
   */
  Animation* LoadAnimation(const std::string& name);
}

#endif // ! ANIMATION_H
