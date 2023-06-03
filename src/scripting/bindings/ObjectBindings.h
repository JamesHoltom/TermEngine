/// @author James Holtom

#ifndef OBJECT_BINDINGS_H
#define OBJECT_BINDINGS_H

#include <glm/glm.hpp>
#include "../../usertypes/game_objects/BaseObject.h"
#include "../../usertypes/game_objects/EventListener.h"
#include "../../usertypes/game_objects/GameObject.h"
#include "../../usertypes/game_objects/TimedFunction.h"
#include "../../usertypes/resources/BaseResource.h"
#include "../../usertypes/resources/Animation.h"
#include "../../usertypes/resources/Audio.h"
#include "../../usertypes/resources/Background.h"
#include "../../usertypes/resources/Font.h"
#include "../../usertypes/resources/ShaderProgram.h"
#include "../../usertypes/CharacterMap.h"
#include "../../usertypes/GameScene.h"
#include "../../usertypes/Timer.h"
#include "../../usertypes/Window.h"
#include "../../utility/SolUtils.h"

SOL_BASE_CLASSES(term_engine::usertypes::EventListener, term_engine::usertypes::BaseObject);
SOL_BASE_CLASSES(term_engine::usertypes::GameObject, term_engine::usertypes::BaseObject);
SOL_BASE_CLASSES(term_engine::usertypes::TimedFunction, term_engine::usertypes::BaseObject);
SOL_BASE_CLASSES(term_engine::usertypes::Animation, term_engine::usertypes::BaseResource);
SOL_BASE_CLASSES(term_engine::usertypes::Audio, term_engine::usertypes::BaseResource);
SOL_BASE_CLASSES(term_engine::usertypes::Background, term_engine::usertypes::BaseResource);
SOL_BASE_CLASSES(term_engine::usertypes::Font, term_engine::usertypes::BaseResource);
SOL_BASE_CLASSES(term_engine::usertypes::ShaderProgram, term_engine::usertypes::BaseResource);
SOL_DERIVED_CLASSES(term_engine::usertypes::BaseObject, term_engine::usertypes::EventListener, term_engine::usertypes::GameObject, term_engine::usertypes::TimedFunction);
SOL_DERIVED_CLASSES(term_engine::usertypes::BaseResource, term_engine::usertypes::Animation, term_engine::usertypes::Audio, term_engine::usertypes::Background, term_engine::usertypes::Font, term_engine::usertypes::ShaderProgram);

namespace term_engine::scripting::bindings {
  /**
   * @brief Binds the usertype classes to the Lua state.
   * 
   * @param[in] state The lua state to bind to.
   */
  void BindUsertypesToState(sol::state& state)
  {
    state.new_usertype<usertypes::BaseObject>(
      "__baseobject",
      sol::meta_function::construct, sol::no_constructor,
      "id", sol::readonly_property(&usertypes::BaseObject::GetObjectId),
      "type", sol::readonly_property(&usertypes::BaseObject::GetObjectType),
      "active", sol::property(&usertypes::BaseObject::IsActive, &usertypes::BaseObject::SetActive),
      "release", &usertypes::BaseObject::FlagForRemoval);

    state.new_usertype<usertypes::BaseResource>(
      "__baseresource",
      sol::meta_function::construct, sol::no_constructor,
      "type", sol::readonly_property(&usertypes::BaseResource::GetResourceType),
      "name", sol::readonly_property(&usertypes::BaseResource::GetName),
      "release", &usertypes::BaseResource::FlagForRemoval);

    state.new_usertype<usertypes::EventListener>(
      "EventListener",
      sol::meta_function::construct, sol::factories(&usertypes::AddEventListenerToScene, &usertypes::AddEventListener),
      sol::call_constructor, sol::factories(&usertypes::AddEventListenerToScene, &usertypes::AddEventListener),
      sol::base_classes, sol::bases<usertypes::BaseObject>(),
      "type", sol::readonly_property(&usertypes::EventListener::GetListenerType),
      "trigger", &usertypes::EventListener::Trigger);

    state.new_usertype<usertypes::GameObject>(
      "GameObject",
      sol::meta_function::construct, sol::factories(&usertypes::AddGameObjectToScene, &usertypes::AddGameObject),
      sol::call_constructor, sol::factories(&usertypes::AddGameObjectToScene, &usertypes::AddGameObject),
      sol::base_classes, sol::bases<usertypes::BaseObject>(),
      "position", sol::property(&usertypes::GameObject::GetPosition, &usertypes::GameObject::SetPosition),
      "size", sol::property(&usertypes::GameObject::GetSize, &usertypes::GameObject::SetSize),
      "data", sol::readonly_property(&usertypes::GameObject::GetData),
      "animation", sol::readonly_property(&usertypes::GameObject::GetAnimation),
      "set", &usertypes::GameObject::Set,
      "moveToScene", &usertypes::GameObject::MoveToGameScene,
      "copyToScene", &usertypes::GameObject::CopyToGameScene);

    state.new_usertype<usertypes::TimedFunction>(
      "TimedFunction",
      sol::meta_function::construct, sol::factories(&usertypes::AddTimedFunction),
      sol::call_constructor, sol::factories(&usertypes::AddTimedFunction),
      sol::base_classes, sol::bases<usertypes::BaseObject>(),
      "active", sol::property(&usertypes::TimedFunction::IsActive, &usertypes::TimedFunction::SetActive),
      "delay", sol::readonly_property(&usertypes::TimedFunction::GetDelay),
      "repeat", sol::readonly_property(&usertypes::TimedFunction::IsRepeatable),
      "timesRepeated", sol::readonly_property(&usertypes::TimedFunction::GetTimesCalled));

    state.new_usertype<usertypes::Animation>(
      "Animation",
      sol::meta_function::construct, sol::factories(&usertypes::LoadAnimation),
      sol::call_constructor, sol::factories(&usertypes::LoadAnimation),
      sol::base_classes, sol::bases<usertypes::BaseResource>(),
      "frameCount", &usertypes::Animation::GetFrameCount,
      "addFrame", sol::overload(
        &usertypes::Animation::PushFrame,
        &usertypes::Animation::AddFrame),
      "removeFrame", &usertypes::Animation::RemoveFrame);

    state.new_usertype<usertypes::Audio>(
      "Audio",
      sol::meta_function::construct, sol::factories(&usertypes::LoadAudio),
      sol::call_constructor, sol::factories(&usertypes::LoadAudio),
      sol::base_classes, sol::bases<usertypes::BaseResource>(),
      "play", &usertypes::Audio::Play,
      "stop", &usertypes::Audio::Stop,
      "pause", &usertypes::Audio::Pause,
      "rewind", &usertypes::Audio::Rewind,
      "seek", &usertypes::Audio::Seek,
      "looping", sol::property(&usertypes::Audio::IsLooping, &usertypes::Audio::SetLooping),
      "pan", sol::property(&usertypes::Audio::GetPan, &usertypes::Audio::SetPan),
      "pitch", sol::property(&usertypes::Audio::GetPitch, &usertypes::Audio::SetPitch),
      "position", sol::property(&usertypes::Audio::GetPosition, &usertypes::Audio::SetPosition),
      "volume", sol::property(&usertypes::Audio::GetVolume, &usertypes::Audio::SetVolume),
      "cursor", sol::readonly_property(&usertypes::Audio::GetCursor),
      "length", sol::readonly_property(&usertypes::Audio::GetLength),
      "paused", sol::readonly_property(&usertypes::Audio::IsPaused),
      "playing", sol::readonly_property(&usertypes::Audio::IsPlaying));
    
    state.new_usertype<usertypes::Background>(
      "Background",
      sol::meta_function::construct, sol::factories(&usertypes::LoadBackground),
      sol::call_constructor, sol::factories(&usertypes::LoadBackground),
      sol::base_classes, sol::bases<usertypes::BaseResource>(),
      "position", sol::property(&usertypes::Background::GetPosition, &usertypes::Background::SetPosition),
      "size", sol::property(&usertypes::Background::GetSize, &usertypes::Background::SetSize),
      "imageSize", sol::readonly_property(&usertypes::Background::GetTextureSize),
      "colour", sol::property(&usertypes::Background::GetColour, &usertypes::Background::SetColour),
      "reset", &usertypes::Background::Reset);

    state.new_usertype<usertypes::Font>(
      "Font",
      sol::meta_function::construct, sol::factories(&usertypes::LoadFont),
      sol::call_constructor, sol::factories(&usertypes::LoadFont),
      sol::base_classes, sol::bases<usertypes::BaseResource>(),
      "filepath", sol::readonly_property(&usertypes::Font::GetName),
      "characterSize", &usertypes::Font::GetCharacterSize);

    state.new_usertype<usertypes::ShaderProgram>(
      "ShaderProgram",
      sol::meta_function::construct, sol::factories(&usertypes::AddShader),
      sol::call_constructor, sol::factories(&usertypes::AddShader),
      sol::base_classes, sol::bases<usertypes::BaseResource>(),
      "setUniform", sol::overload(
        &usertypes::ShaderProgram::SetUniform<float>,
        &usertypes::ShaderProgram::SetUniform<int>,
        &usertypes::ShaderProgram::SetUniformVector<glm::vec2>,
        &usertypes::ShaderProgram::SetUniformVector<glm::ivec2>,
        &usertypes::ShaderProgram::SetUniformVector<glm::uvec2>,
        &usertypes::ShaderProgram::SetUniformVector<glm::vec3>,
        &usertypes::ShaderProgram::SetUniformVector<glm::ivec3>,
        &usertypes::ShaderProgram::SetUniformVector<glm::uvec3>,
        &usertypes::ShaderProgram::SetUniformVector<glm::vec4>,
        &usertypes::ShaderProgram::SetUniformVector<glm::ivec4>,
        &usertypes::ShaderProgram::SetUniformVector<glm::ivec4>));
    
    state.new_usertype<usertypes::AnimationFrame>(
      "AnimationFrame",
      sol::meta_function::construct, sol::constructors<void(const sol::table&, const glm::ivec2&),
                                                       void(const sol::table&, const glm::ivec2&, const glm::ivec2&),
                                                       void(const sol::table&, const glm::ivec2&, const glm::ivec2&, uint32_t)>(),
      sol::call_constructor, sol::constructors<void(const sol::table&, const glm::ivec2&),
                                               void(const sol::table&, const glm::ivec2&, const glm::ivec2&),
                                               void(const sol::table&, const glm::ivec2&, const glm::ivec2&, uint32_t)>(),
      "data", &usertypes::AnimationFrame::data_,
      "size", &usertypes::AnimationFrame::size_,
      "offset", &usertypes::AnimationFrame::offset_,
      "added_duration", &usertypes::AnimationFrame::added_duration_);

    state.new_usertype<usertypes::AnimationState>(
      "AnimationState",
      sol::meta_function::construct, sol::no_constructor,
      sol::call_constructor, sol::no_constructor,
      "isPlaying", sol::readonly_property(&usertypes::AnimationState::IsPlaying),
      "looping", sol::property(&usertypes::AnimationState::IsLooping, &usertypes::AnimationState::SetLooping),
      "yoyoing", sol::property(&usertypes::AnimationState::IsYoyoing, &usertypes::AnimationState::SetYoyoing),
      "reversing", sol::property(&usertypes::AnimationState::IsReversing, &usertypes::AnimationState::SetReversing),
      "frameNumber", sol::readonly_property(&usertypes::AnimationState::GetFrameNumber),
      "play", &usertypes::AnimationState::Play,
      "stop", &usertypes::AnimationState::Stop,
      "seek", &usertypes::AnimationState::Seek,
      "currentAnimation", sol::readonly_property(&usertypes::AnimationState::GetCurrentAnimation),
      "hasAnimationsQueued", sol::readonly_property(&usertypes::AnimationState::HasAnimationsQueued),
      "addToQueue", &usertypes::AnimationState::AddToQueue,
      "clearQueue", &usertypes::AnimationState::ClearQueue);

    state.new_usertype<usertypes::CharacterParams>(
      "Character",
      sol::meta_function::construct, sol::constructors<void(),
                                                       void(const usertypes::CharacterParams&),
                                                       void(char16_t, const glm::vec4&, const glm::vec4&)>(),
      sol::call_constructor, sol::constructors<void(),
                                               void(const usertypes::CharacterParams&),
                                               void(char16_t, const glm::vec4&, const glm::vec4&)>(),
      sol::meta_function::equal_to, [](const usertypes::CharacterParams& lhs, const usertypes::CharacterParams& rhs) { return lhs == rhs; },
      "character", &usertypes::CharacterParams::character_,
      "foregroundColour", &usertypes::CharacterParams::foreground_colour_,
      "backgroundColour", &usertypes::CharacterParams::background_colour_);

    state.new_usertype<usertypes::CharacterMap>(
      "CharacterMap",
      sol::meta_function::construct, sol::no_constructor,
      "position", sol::property(&usertypes::CharacterMap::GetPosition, &usertypes::CharacterMap::SetPosition),
      "size", sol::property(&usertypes::CharacterMap::GetSize, &usertypes::CharacterMap::SetSize),
      "hideEmptyCharacters", sol::property(&usertypes::CharacterMap::ShouldEmptyCharactersHaveBackground, &usertypes::CharacterMap::SetEmptyCharacterBackground));
    
    state.new_usertype<usertypes::GameScene>(
      "GameScene",
      sol::meta_function::construct, sol::factories(&usertypes::AddGameScene),
      sol::call_constructor, sol::factories(&usertypes::AddGameScene),
      "name", sol::readonly_property(&usertypes::GameScene::GetName),
      "background", sol::property(&usertypes::GameScene::GetBackground, &usertypes::GameScene::SetBackground),
      "charmap", sol::readonly_property(&usertypes::GameScene::GetCharacterMap),
      "font", sol::property(&usertypes::GameScene::GetFont, &usertypes::GameScene::SetFont),
      "fontSize", sol::property(&usertypes::GameScene::GetFontSize, &usertypes::GameScene::SetFontSize),
      "backgroundShader", sol::property(&usertypes::GameScene::GetBackgroundShader, &usertypes::GameScene::SetBackgroundShader),
      "textShader", sol::property(&usertypes::GameScene::GetTextShader, &usertypes::GameScene::SetTextShader),
      "window", sol::readonly_property(&usertypes::GameScene::GetWindow),
      "close", &usertypes::GameScene::FlagForRemoval,
      "preventClose", &usertypes::GameScene::UnflagForRemoval,
      "resizeToCharacterMap", &usertypes::GameScene::ResizeToFitCharacterMap,
      "resizeToWindow", &usertypes::GameScene::ResizeToFitWindow);
    
    state.new_usertype<usertypes::Timer>(
      "Timer",
      sol::meta_function::construct, sol::constructors<void()>(),
      sol::call_constructor, sol::constructors<void()>(),
      "start", &usertypes::Timer::Start,
      "stop", &usertypes::Timer::Stop,
      "pause", &usertypes::Timer::Pause,
      "resume", &usertypes::Timer::Resume,
      "interval", sol::readonly_property(&usertypes::Timer::GetIntervalElapsed),
      "started", sol::readonly_property(&usertypes::Timer::IsStarted),
      "paused", sol::readonly_property(&usertypes::Timer::IsPaused),
      "duration", sol::readonly_property(&usertypes::Timer::GetDuration),
      "pausedDuration", sol::readonly_property(&usertypes::Timer::GetPauseDuration));

    state.new_usertype<usertypes::Window>(
      "Window",
      sol::meta_function::construct, sol::no_constructor,
      "title", sol::property(&usertypes::Window::GetTitle, &usertypes::Window::SetTitle),
      "position", sol::property(&usertypes::Window::GetPosition, &usertypes::Window::SetPosition),
      "centerPosition", &usertypes::Window::CenterPosition,
      "size", sol::property(&usertypes::Window::GetSize, &usertypes::Window::SetSize),
      "resizable", sol::property(&usertypes::Window::IsResizable, &usertypes::Window::SetResizable),
      "minimised", sol::property(&usertypes::Window::IsMinimised, &usertypes::Window::Minimise),
      "maximised", sol::property(&usertypes::Window::IsMaximised, &usertypes::Window::Maximise),
      "restore", &usertypes::Window::Restore,
      "show", &usertypes::Window::Show,
      "hide", &usertypes::Window::Hide,
      "captured", sol::property(&usertypes::Window::IsMouseGrabbed, &usertypes::Window::SetMouseGrab),
      "clearColour", sol::property(&usertypes::Window::GetClearColour, &usertypes::Window::SetClearColour),
      "closeBehaviour", sol::property(&usertypes::Window::GetCloseBehaviour, &usertypes::Window::SetCloseBehaviour));
  }
}

#endif // ! OBJECT_BINDINGS_H
