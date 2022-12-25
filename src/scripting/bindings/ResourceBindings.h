/// @author James Holtom

#ifndef RESOURCE_BINDINGS_H
#define RESOURCE_BINDINGS_H

#include "../../resources/Audio.h"
#include "../../resources/Background.h"
#include "../../resources/FontAtlas.h"
#include "../../resources/Window.h"
#include "../../utility/GLUtils.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /**
   * @brief Binds the resource-related script functions to the Lua state.
   * 
   * @param[in] state The lua state to bind to.
   */
  void BindResourcesToState(sol::state& state)
  {
    state.new_usertype<resources::Audio>(
      "Audio",
      sol::meta_function::construct, sol::factories(&resources::Audio::Add),
      sol::call_constructor, sol::factories(&resources::Audio::Add),
      sol::meta_function::garbage_collect, sol::destructor(&resources::Audio::Remove),
      "play", &resources::Audio::Play,
      "stop", &resources::Audio::Stop,
      "pause", &resources::Audio::Pause,
      "resume", &resources::Audio::Resume,
      "looping", sol::property(&resources::Audio::IsLooping, &resources::Audio::SetLooping),
      "pan", sol::property(&resources::Audio::GetPan, &resources::Audio::SetPan),
      "pitch", sol::property(&resources::Audio::GetPitch, &resources::Audio::SetPitch),
      "position", sol::property(&resources::Audio::GetPosition, &resources::Audio::SetPosition),
      "volume", sol::property(&resources::Audio::GetVolume, &resources::Audio::SetVolume),
      "paused", sol::property(&resources::Audio::IsPaused),
      "playing", sol::property(&resources::Audio::IsPlaying),
      "filepath", sol::property(&resources::Audio::GetFilePath),
      "cursor", sol::property(&resources::Audio::GetCursorSeconds),
      "length", sol::property(&resources::Audio::GetLengthSeconds),
      "release", &resources::Audio::Remove);

    state.create_named_table(
      "audio",
      "trigger", &resources::Audio::PlaySound,
      "count", &resources::Audio::Count,
      "exists", &resources::Audio::Exists
    );

    state.create_named_table(
      "background",
      "set", sol::overload(sol::resolve<void(const std::string&, const glm::vec2&, const glm::vec3&)>(&background::SetBackground),
                           sol::resolve<void(const std::string&, const glm::vec2&, const glm::vec4&)>(&background::SetBackground)),
      "remove", &background::RemoveBackground);
    
    state.create_named_table(
      "font",
      "set", &fonts::SetFont,
      "getPath", &fonts::GetFontPath,
      "getSize", &fonts::GetFontSize,
      "getCharacterSize", &fonts::GetCharacterSize,
      "setCharacterSize", &fonts::SetCharacterSize,
      "resetCharacterSize", &fonts::ResetCharacterSize,
      "getDefaultPath", &fonts::GetDefaultFontPath,
      "getDefaultSize", &fonts::GetDefaultFontSize,
      "tabSize", fonts::tab_size);

    state.create_named_table(
      "window",
      "getPosition", &system::GetWindowPosition,
      "setPosition", &system::SetWindowPosition,
      "centerPosition", &system::CenterWindowPosition,
      "getSize", &system::GetWindowSize,
      "setSize", &system::SetWindowSize,
      "fitToView", &system::ResizeWindowToView,
      "isResizable", &system::IsWindowResizable,
      "setResizable", &system::SetWindowResizable,
      "isMinimised", &system::IsWindowMinimised,
      "isMaximised", &system::IsWindowMaximised,
      "minimise", &system::MinimiseWindow,
      "maximise", &system::MaximiseWindow,
      "restore", &system::RestoreWindow,
      "show", &system::ShowWindow,
      "hide", &system::HideWindow,
      "isCaptured", &system::IsMouseGrabbed,
      "setCapture", &system::SetMouseGrab,
      "getClearColour", &system::GetWindowClearColour,
      "setClearColour", sol::overload(sol::resolve<void(const glm::vec3&)>(&system::SetWindowClearColour),
                                      sol::resolve<void(const glm::vec4&)>(&system::SetWindowClearColour)),
      "isVsync", &system::IsVsyncEnabled,
      "setVsync", &system::SetVsync);
  }
}

#endif // ! RESOURCE_BINDINGS_H
