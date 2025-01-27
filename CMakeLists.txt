cmake_minimum_required(VERSION 3.16.0)

project("TermEngine" VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_FLAGS, "${CMAKE_CXX_FLAGS} /utf8 /Zc:char8_t-")
set(CMAKE_EXE_LINKER_FLAGS, "-lpthread -lm -ldl -lformat")
set(CMAKE_TOOLCHAIN_FILE, "$CMAKE_TOOLCHAIN_FILE")

find_package(GLEW)
find_package(Lua)
find_package(SDL2)

add_subdirectory(vendor/cxxopts)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/sol2)

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
  "./src/usertypes/Flaggable.cc"
  "./src/usertypes/GameScene.cc"
  "./src/usertypes/GameWindow.cc"
  "./src/usertypes/Timer.cc"
  "./src/usertypes/Window.cc"
  "./src/usertypes/game_objects/BaseObject.cc"
  "./src/usertypes/game_objects/GameObject.cc"
  "./src/usertypes/game_objects/TimedFunction.cc"
  "./src/usertypes/resources/BaseResource.cc"
  "./src/usertypes/resources/Animation.cc"
  "./src/usertypes/resources/Audio.cc"
  "./src/usertypes/resources/Font.cc"
  "./src/usertypes/resources/Image.cc"
  "./src/usertypes/resources/ShaderProgram.cc"
  "./src/utility/AudioUtils.cc"
  "./src/utility/ConversionUtils.cc"
  "./src/utility/FTUtils.cc"
  "./src/utility/GLUtils.cc"
  "./src/utility/ImGuiUtils.cc"
  "./src/utility/LogUtils.cc"
  "./src/utility/SDLUtils.cc"
  "./src/utility/SolUtils.cc"
  "./vendor/miniaudio.cc"
  "./vendor/stbi.cc"
  "./vendor/imgui/imgui.cpp"
  "./vendor/imgui/imgui_demo.cpp"
  "./vendor/imgui/imgui_draw.cpp"
  "./vendor/imgui/imgui_tables.cpp"
  "./vendor/imgui/imgui_widgets.cpp"
  "./vendor/imgui/backends/imgui_impl_opengl3.cpp"
  "./vendor/imgui/backends/imgui_impl_sdl2.cpp"
  "./vendor/imgui/misc/freetype/imgui_freetype.cpp"
)

target_include_directories("${PROJECT_NAME}" PRIVATE
  "/usr/include/freetype2/"
  "./src"
  "./vendor/imgui"
  ${LUA_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIR}
)
target_link_libraries("${PROJECT_NAME}" PRIVATE
  cxxopts::cxxopts
  freetype
  GLEW::GLEW
  glm::glm
  ${LUA_LIBRARIES}
  SDL2::SDL2 SDL2::SDL2main SDL2::SDL2-static
  sol2
)

target_compile_options("${PROJECT_NAME}" PRIVATE -W -Wall -Wextra -lpthread -lm -ldl -lformat)
