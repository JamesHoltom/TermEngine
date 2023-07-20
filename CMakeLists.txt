cmake_minimum_required(VERSION 3.16.0)

project("TermEngine" VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_FLAGS, "${CMAKE_CXX_FLAGS} /utf8 /Zc:char8_t-")
set(CMAKE_EXE_LINKER_FLAGS, "-lpthread -lm -ldl")
set(CMAKE_TOOLCHAIN_FILE, "$CMAKE_TOOLCHAIN_FILE")
set(VCPKG_INCLUDE_DIR, "$ENV{VCPKG_ROOT}/installed/x64-linux/include")

find_package(cxxopts CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable("${PROJECT_NAME}"
  "./src/main.cc"
  "./src/Application.cc"
  "./src/events/InputManager.cc"
  "./src/events/Listener.cc"
  "./src/rendering/Buffer.cc"
  "./src/rendering/Texture.cc"
  "./src/rendering/TexturePacker.cc"
  "./src/scripting/ScriptingInterface.cc"
  "./src/system/CLArguments.cc"
  "./src/system/FileFunctions.cc"
  "./src/system/FPSManager.cc"
  "./src/usertypes/Background.cc"
  "./src/usertypes/Character.cc"
  "./src/usertypes/CharacterMap.cc"
  "./src/usertypes/EventListener.cc"
  "./src/usertypes/GameScene.cc"
  "./src/usertypes/Timer.cc"
  "./src/usertypes/Window.cc"
  "./src/usertypes/game_objects/BaseObject.cc"
  "./src/usertypes/game_objects/GameObject.cc"
  "./src/usertypes/game_objects/TimedFunction.cc"
  "./src/usertypes/resources/BaseResource.cc"
  "./src/usertypes/resources/Animation.cc"
  "./src/usertypes/resources/Audio.cc"
  "./src/usertypes/resources/Font.cc"
  "./src/usertypes/resources/ShaderProgram.cc"
  "./src/utility/AudioUtils.cc"
  "./src/utility/ConversionUtils.cc"
  "./src/utility/FTUtils.cc"
  "./src/utility/GLUtils.cc"
  "./src/utility/ImGuiUtils.cc"
  "./src/utility/LogUtils.cc"
  "./src/utility/SDLUtils.cc"
  "./src/utility/SpdlogUtils.cc"
  "./src/vendor/miniaudio.cc"
  "./src/vendor/stbi.cc"
  "./src/vendor/imgui/imgui.cpp"
  "./src/vendor/imgui/imgui_demo.cpp"
  "./src/vendor/imgui/imgui_draw.cpp"
  "./src/vendor/imgui/imgui_tables.cpp"
  "./src/vendor/imgui/imgui_widgets.cpp"
  "./src/vendor/imgui/backends/imgui_impl_opengl3.cpp"
  "./src/vendor/imgui/backends/imgui_impl_sdl2.cpp"
  "./src/vendor/imgui/misc/freetype/imgui_freetype.cpp"
)

target_include_directories("${PROJECT_NAME}" PRIVATE
  "./src"
  "./src/vendor/imgui"
  ${LUA_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIR}
  "${VCPKG_INCLUDE_DIR}/sol"
  "${VCPKG_INCLUDE_DIR}/spdlog"
)
target_link_libraries("${PROJECT_NAME}" PRIVATE
  cxxopts::cxxopts
  fmt::fmt
  freetype
  GLEW::GLEW
  glm::glm
  ${LUA_LIBRARIES}
  SDL2::SDL2 SDL2::SDL2main SDL2::SDL2-static
  sol2
  spdlog::spdlog
  spdlog::spdlog_header_only
)
