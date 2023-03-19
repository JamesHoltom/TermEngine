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

SOL_BASE_CLASSES(term_engine::objects::Audio, term_engine::objects::BaseObject);
SOL_BASE_CLASSES(term_engine::objects::EventListener, term_engine::objects::BaseObject);
SOL_BASE_CLASSES(term_engine::objects::GameObject, term_engine::objects::BaseObject);
SOL_BASE_CLASSES(term_engine::objects::GameScene, term_engine::objects::BaseObject);
SOL_BASE_CLASSES(term_engine::objects::TimedFunction, term_engine::objects::BaseObject);
SOL_DERIVED_CLASSES(term_engine::objects::BaseObject, term_engine::objects::Audio, term_engine::objects::EventListener, term_engine::objects::GameObject, term_engine::objects::GameScene, term_engine::objects::TimedFunction);

namespace term_engine::scripting::bindings {
  /**
   * @brief Binds the object-related classes to the Lua state.
   * 
   * @param[in] state The lua state to bind to.
   */
  void BindObjectsToState(sol::state& state)
  {
    state.new_usertype<objects::BaseObject>(
      "__baseobject",
      sol::meta_function::construct, sol::no_constructor,
      "id", sol::readonly_property(&objects::BaseObject::GetObjectId),
      "active", sol::property(&objects::BaseObject::IsActive, &objects::BaseObject::SetActive),
      "release", &objects::BaseObject::FlagForRemoval);

    state.new_usertype<objects::Audio>(
      "Audio",
      sol::meta_function::construct, sol::factories(&objects::AddAudio),
      sol::call_constructor, sol::factories(&objects::AddAudio),
      sol::base_classes, sol::bases<objects::BaseObject>(),
      "play", &objects::Audio::Play,
      "stop", &objects::Audio::Stop,
      "pause", &objects::Audio::Pause,
      "rewind", &objects::Audio::Rewind,
      "seek", &objects::Audio::Seek,
      "looping", sol::property(&objects::Audio::IsLooping, &objects::Audio::SetLooping),
      "pan", sol::property(&objects::Audio::GetPan, &objects::Audio::SetPan),
      "pitch", sol::property(&objects::Audio::GetPitch, &objects::Audio::SetPitch),
      "position", sol::property(&objects::Audio::GetPosition, &objects::Audio::SetPosition),
      "volume", sol::property(&objects::Audio::GetVolume, &objects::Audio::SetVolume),
      "paused", sol::readonly_property(&objects::Audio::IsPaused),
      "playing", sol::readonly_property(&objects::Audio::IsPlaying),
      "filepath", sol::readonly_property(&objects::Audio::GetFilePath),
      "cursor", sol::readonly_property(&objects::Audio::GetCursorSeconds),
      "length", sol::readonly_property(&objects::Audio::GetLengthSeconds));
    
    state.new_usertype<objects::EventListener>(
      "EventListener",
      sol::meta_function::construct, sol::factories(&objects::AddEventListenerToScene, &objects::AddEventListener),
      sol::call_constructor, sol::factories(&objects::AddEventListenerToScene, &objects::AddEventListener),
      sol::base_classes, sol::bases<objects::BaseObject>(),
      "type", sol::readonly_property(&objects::EventListener::GetListenerType),
      "trigger", &objects::EventListener::Trigger);

    state.new_usertype<objects::GameObject>(
      "GameObject",
      sol::meta_function::construct, sol::factories(&objects::AddGameObjectToScene, &objects::AddGameObject),
      sol::call_constructor, sol::factories(&objects::AddGameObjectToScene, &objects::AddGameObject),
      sol::base_classes, sol::bases<objects::BaseObject>(),
      "position", sol::property(&objects::GameObject::GetPosition, &objects::GameObject::SetPosition),
      "size", sol::property(&objects::GameObject::GetSize, &objects::GameObject::SetSize),
      "data", sol::readonly_property(&objects::GameObject::GetData),
      "set", &objects::GameObject::Set,
      "moveToScene", &objects::GameObject::MoveToGameScene,
      "copyToScene", &objects::GameObject::CopyToGameScene);

    state.new_usertype<objects::GameScene>(
      "GameScene",
      sol::meta_function::construct, sol::factories(&objects::AddGameScene),
      sol::call_constructor, sol::factories(&objects::AddGameScene),
      sol::base_classes, sol::bases<objects::BaseObject>(),
      "name", sol::readonly_property(&objects::GameScene::GetName),
      "background", sol::readonly_property(&objects::GameScene::GetBackground),
      "charmap", sol::readonly_property(&objects::GameScene::GetCharacterMap),
      "font", sol::readonly_property(&objects::GameScene::GetFontAtlas),
      "fontSize", sol::property(&objects::GameScene::GetFontSize, &objects::GameScene::SetFontSize),
      "backgroundShader", sol::readonly_property(&objects::GameScene::GetBackgroundShader),
      "textShader", sol::readonly_property(&objects::GameScene::GetTextShader),
      "window", sol::readonly_property(&objects::GameScene::GetWindow),
      "preventClose", &objects::GameScene::UnflagForRemoval,
      "resizeToFit", &objects::GameScene::ResizeToFit);
    
    state.new_usertype<objects::TimedFunction>(
      "TimedFunction",
      sol::meta_function::construct, sol::factories(&objects::AddTimedFunction),
      sol::call_constructor, sol::factories(&objects::AddTimedFunction),
      sol::base_classes, sol::bases<objects::BaseObject>(),
      "active", sol::property(&objects::TimedFunction::IsActive, &objects::TimedFunction::SetActive),
      "delay", sol::readonly_property(&objects::TimedFunction::GetDelay),
      "repeat", sol::readonly_property(&objects::TimedFunction::IsRepeatable),
      "timesRepeated", sol::readonly_property(&objects::TimedFunction::GetTimesRepeated));

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
      "size", sol::property(&rendering::CharacterMap::GetSize, &rendering::CharacterMap::SetSize));
    
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
        &rendering::ShaderProgram::SetUniform<float>,
        &rendering::ShaderProgram::SetUniform<int>,
        &rendering::ShaderProgram::SetUniformVector<glm::vec2>,
        &rendering::ShaderProgram::SetUniformVector<glm::ivec2>,
        &rendering::ShaderProgram::SetUniformVector<glm::uvec2>,
        &rendering::ShaderProgram::SetUniformVector<glm::vec3>,
        &rendering::ShaderProgram::SetUniformVector<glm::ivec3>,
        &rendering::ShaderProgram::SetUniformVector<glm::uvec3>,
        &rendering::ShaderProgram::SetUniformVector<glm::vec4>,
        &rendering::ShaderProgram::SetUniformVector<glm::ivec4>,
        &rendering::ShaderProgram::SetUniformVector<glm::ivec4>));
    
    state.new_usertype<timing::Timer>(
      "Timer",
      sol::meta_function::construct, sol::constructors<void()>(),
      sol::call_constructor, sol::constructors<void()>(),
      "start", &timing::Timer::Start,
      "stop", &timing::Timer::Stop,
      "pause", &timing::Timer::Pause,
      "resume", &timing::Timer::Resume,
      "interval", sol::readonly_property(&timing::Timer::GetIntervalElapsed),
      "started", sol::readonly_property(&timing::Timer::IsStarted),
      "paused", sol::readonly_property(&timing::Timer::IsPaused),
      "duration", sol::readonly_property(&timing::Timer::GetDuration),
      "paused_duration", sol::readonly_property(&timing::Timer::GetPauseDuration));

    state.new_usertype<rendering::GameWindow>(
      "Window",
      sol::meta_function::construct, sol::no_constructor,
      "title", sol::property(&rendering::GameWindow::GetTitle, &rendering::GameWindow::SetTitle),
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
      "clearColour", sol::property(&rendering::GameWindow::GetClearColour, &rendering::GameWindow::SetClearColour),
      "closeBehaviour", sol::property(&rendering::GameWindow::GetCloseBehaviour, &rendering::GameWindow::SetCloseBehaviour));
  }
}

#endif // ! OBJECT_BINDINGS_H
