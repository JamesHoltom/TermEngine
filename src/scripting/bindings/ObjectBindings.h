/// @author James Holtom

#ifndef OBJECT_BINDINGS_H
#define OBJECT_BINDINGS_H

#include <glm/glm.hpp>
#include "../../objects/BaseObject.h"
#include "../../objects/Audio.h"
#include "../../objects/EventListener.h"
#include "../../objects/GameObject.h"
#include "../../objects/GameScene.h"
#include "../../objects/TimedFunction.h"
#include "../../utility/SolUtils.h"

SOL_BASE_CLASSES(term_engine::objects::AudioProxy, term_engine::objects::BaseProxy);
SOL_BASE_CLASSES(term_engine::objects::EventListenerProxy, term_engine::objects::BaseProxy);
SOL_BASE_CLASSES(term_engine::objects::GameObjectProxy, term_engine::objects::BaseProxy);
SOL_BASE_CLASSES(term_engine::objects::GameSceneProxy, term_engine::objects::BaseProxy);
SOL_BASE_CLASSES(term_engine::objects::TimedFunctionProxy, term_engine::objects::BaseProxy);
SOL_DERIVED_CLASSES(term_engine::objects::BaseProxy, term_engine::objects::AudioProxy, term_engine::objects::EventListenerProxy, term_engine::objects::GameObjectProxy, term_engine::objects::GameSceneProxy, term_engine::objects::TimedFunctionProxy);

namespace term_engine::scripting::bindings {
  /**
   * @brief Binds the object-related classes to the Lua state.
   * 
   * @param[in] state The lua state to bind to.
   */
  void BindObjectsToState(sol::state& state)
  {
    state.new_usertype<objects::BaseProxy>(
      "__proxy",
      sol::meta_function::construct, sol::no_constructor,
      "id", sol::readonly_property(&objects::BaseProxy::GetObjectId),
      "active", sol::property(&objects::BaseProxy::IsActive, &objects::BaseProxy::SetActive),
      "release", &objects::BaseProxy::Release);

    state.new_usertype<objects::AudioProxy>(
      "Audio",
      sol::meta_function::construct, sol::factories(&objects::AddAudio),
      sol::call_constructor, sol::factories(&objects::AddAudio),
      sol::base_classes, sol::bases<objects::BaseProxy>(),
      "play", &objects::AudioProxy::Play,
      "stop", &objects::AudioProxy::Stop,
      "pause", &objects::AudioProxy::Pause,
      "resume", &objects::AudioProxy::Resume,
      "looping", sol::property(&objects::AudioProxy::IsLooping, &objects::AudioProxy::SetLooping),
      "pan", sol::property(&objects::AudioProxy::GetPan, &objects::AudioProxy::SetPan),
      "pitch", sol::property(&objects::AudioProxy::GetPitch, &objects::AudioProxy::SetPitch),
      "position", sol::property(&objects::AudioProxy::GetPosition, &objects::AudioProxy::SetPosition),
      "volume", sol::property(&objects::AudioProxy::GetVolume, &objects::AudioProxy::SetVolume),
      "paused", sol::readonly_property(&objects::AudioProxy::IsPaused),
      "playing", sol::readonly_property(&objects::AudioProxy::IsPlaying),
      "filepath", sol::readonly_property(&objects::AudioProxy::GetFilePath),
      "cursor", sol::readonly_property(&objects::AudioProxy::GetCursorSeconds),
      "length", sol::readonly_property(&objects::AudioProxy::GetLengthSeconds));
    
    state.new_usertype<objects::EventListenerProxy>(
      "EventListener",
      sol::meta_function::construct, sol::factories(&objects::AddEventListenerToScene, &objects::AddEventListener),
      sol::call_constructor, sol::factories(&objects::AddEventListenerToScene, &objects::AddEventListener),
      sol::base_classes, sol::bases<objects::BaseProxy>(),
      "type", sol::readonly_property(&objects::EventListenerProxy::GetListenerType),
      "trigger", &objects::EventListenerProxy::Trigger);

    state.new_usertype<objects::GameObjectProxy>(
      "GameObject",
      sol::meta_function::construct, sol::factories(&objects::AddGameObjectToScene, &objects::AddGameObject),
      sol::call_constructor, sol::factories(&objects::AddGameObjectToScene, &objects::AddGameObject),
      sol::base_classes, sol::bases<objects::BaseProxy>(),
      "position", sol::property(&objects::GameObjectProxy::GetPosition, &objects::GameObjectProxy::SetPosition),
      "size", sol::property(&objects::GameObjectProxy::GetSize, &objects::GameObjectProxy::SetSize),
      "data", sol::readonly_property(&objects::GameObjectProxy::GetData),
      "set", &objects::GameObjectProxy::Set,
      "moveToScene", &objects::GameObjectProxy::MoveToGameScene,
      "copyToScene", &objects::GameObjectProxy::CopyToGameScene);

    state.new_usertype<objects::GameSceneProxy>(
      "GameScene",
      sol::meta_function::construct, sol::factories(&objects::AddGameScene),
      sol::call_constructor, sol::factories(&objects::AddGameScene),
      sol::base_classes, sol::bases<objects::BaseProxy>(),
      "name", sol::readonly_property(&objects::GameSceneProxy::GetName),
      "background", sol::readonly_property(&objects::GameSceneProxy::GetBackground),
      "charmap", sol::readonly_property(&objects::GameSceneProxy::GetCharacterMap),
      "font", sol::readonly_property(&objects::GameSceneProxy::GetFontAtlas),
      "backgroundShader", sol::readonly_property(&objects::GameSceneProxy::GetBackgroundShader),
      "textShader", sol::readonly_property(&objects::GameSceneProxy::GetTextShader),
      "window", sol::readonly_property(&objects::GameSceneProxy::GetWindow),
      "close", &objects::GameSceneProxy::FlagRemoval,
      "preventClose", &objects::GameSceneProxy::UnflagRemoval,
      "resizeToFit", &objects::GameSceneProxy::ResizeToFit);
    
    state.new_usertype<objects::TimedFunctionProxy>(
      "TimedFunction",
      sol::meta_function::construct, sol::factories(&objects::AddTimedFunction),
      sol::call_constructor, sol::factories(&objects::AddTimedFunction),
      sol::base_classes, sol::bases<objects::BaseProxy>(),
      "active", sol::property(&objects::TimedFunctionProxy::IsActive, &objects::TimedFunctionProxy::SetActive),
      "delay", sol::readonly_property(&objects::TimedFunctionProxy::GetDelay),
      "repeat", sol::readonly_property(&objects::TimedFunctionProxy::IsRepeatable),
      "timesRepeated", sol::readonly_property(&objects::TimedFunctionProxy::GetTimesRepeated));

    state.new_usertype<rendering::Background>(
      "Background",
      sol::meta_function::construct, sol::no_constructor,
      "position", sol::property(&rendering::Background::GetPosition, &rendering::Background::SetPosition),
      "size", sol::property(&rendering::Background::GetSize, &rendering::Background::SetSize),
      "imageSize", sol::readonly_property(&rendering::Background::GetTextureSize),
      "colour", sol::property(&rendering::Background::GetColour, &rendering::Background::SetColour),
      "reset", &rendering::Background::Reset,
      "load", &rendering::Background::Load,
      "unload", &rendering::Background::Unload);

    state.new_usertype<rendering::CharacterParams>(
      "Character",
      sol::meta_function::construct, sol::constructors<void(),
                                                        void(const rendering::CharacterParams&),
                                                        void(const char&, const glm::vec4&, const glm::vec4&)>(),
      sol::call_constructor, sol::constructors<void(),
                                                void(const rendering::CharacterParams&),
                                                void(const char&, const glm::vec4&, const glm::vec4&)>(),
      sol::meta_function::equal_to, [](const rendering::CharacterParams& lhs, const rendering::CharacterParams& rhs) { return lhs == rhs; },
      "character", &rendering::CharacterParams::character_,
      "foreground_colour", &rendering::CharacterParams::foreground_colour_,
      "background_colour", &rendering::CharacterParams::background_colour_);

    state.new_usertype<rendering::CharacterMap>(
      "CharacterMap",
      sol::meta_function::construct, sol::no_constructor,
      "position", sol::property(&rendering::CharacterMap::GetPosition, &rendering::CharacterMap::SetPosition),
      "size", sol::property(&rendering::CharacterMap::GetSize, &rendering::CharacterMap::SetSize),
      "fontSize", sol::property(&rendering::CharacterMap::GetFontSize, &rendering::CharacterMap::SetFontSize));
    
    state.new_usertype<rendering::FontAtlas>(
      "FontAtlas",
      sol::meta_function::construct, sol::no_constructor,
      "filepath", sol::readonly_property(&rendering::FontAtlas::GetFilePath),
      "characterSize", &rendering::FontAtlas::GetCharacterSize);

    state.new_usertype<rendering::ShaderProgram>(
      "ShaderProgram",
      sol::meta_function::construct, sol::no_constructor,
      "attach", &rendering::ShaderProgram::AttachFile,
      "link", &rendering::ShaderProgram::Link,
      "linked", sol::readonly_property(&rendering::ShaderProgram::IsLinked),
      "setUniform", sol::overload(
        &rendering::ShaderProgram::SetUniform<GLfloat>,
        &rendering::ShaderProgram::SetUniform<GLint>,
        &rendering::ShaderProgram::SetUniform<GLuint>,
        &rendering::ShaderProgram::SetUniform<glm::vec2>,
        &rendering::ShaderProgram::SetUniform<glm::ivec2>,
        &rendering::ShaderProgram::SetUniform<glm::uvec2>,
        &rendering::ShaderProgram::SetUniform<glm::vec3>,
        &rendering::ShaderProgram::SetUniform<glm::ivec3>,
        &rendering::ShaderProgram::SetUniform<glm::uvec3>,
        &rendering::ShaderProgram::SetUniform<glm::vec4>,
        &rendering::ShaderProgram::SetUniform<glm::ivec4>,
        &rendering::ShaderProgram::SetUniform<glm::ivec4>));
    
    state.new_usertype<timing::Timer>(
      "Timer",
      sol::meta_function::construct, sol::constructors<void()>(),
      sol::call_constructor, sol::constructors<void()>(),
      "start", &timing::Timer::Start,
      "stop", &timing::Timer::Stop,
      "pause", &timing::Timer::Pause,
      "resume", &timing::Timer::Resume,
      "intervalMs", sol::readonly_property(&timing::Timer::GetIntervalElapsedMs),
      "intervalSecs", sol::readonly_property(&timing::Timer::GetIntervalElapsed),
      "started", sol::readonly_property(&timing::Timer::IsStarted),
      "paused", sol::readonly_property(&timing::Timer::IsPaused),
      "duration", sol::readonly_property(&timing::Timer::GetDuration),
      "paused_duration", sol::readonly_property(&timing::Timer::GetPauseDuration));

    state.create_named_table(
      "Window",
      sol::meta_function::construct, sol::no_constructor,
      "position", sol::property(&rendering::GameWindow::GetPosition, &rendering::GameWindow::SetPosition),
      "centerPosition", &rendering::GameWindow::CenterPosition,
      "size", sol::property(&rendering::GameWindow::GetSize, &rendering::GameWindow::SetSize),
      "resizable", sol::property(&rendering::GameWindow::IsResizable, &rendering::GameWindow::SetResizable),
      "minimised", sol::property(&rendering::GameWindow::IsMinimised, &rendering::GameWindow::Minimise),
      "maximised", sol::property(&rendering::GameWindow::IsMaximised, &rendering::GameWindow::Maximise),
      "restore", &rendering::GameWindow::Restore,
      "show", &rendering::GameWindow::Show,
      "hide", &rendering::GameWindow::Hide,
      "captured", sol::property(&rendering::GameWindow::IsMouseGrabbed, &rendering::GameWindow::SetMouseGrab),
      "clearColour", sol::property(&rendering::GameWindow::GetClearColour, &rendering::GameWindow::SetClearColour));
  }
}

#endif // ! OBJECT_BINDINGS_H
