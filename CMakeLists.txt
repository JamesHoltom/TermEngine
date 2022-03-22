cmake_minimum_required(VERSION 3.16.0)

project("TermEngine" VERSION 0.2.0 LANGUAGES CXX C)

#include(CTest)
#enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_TOOLCHAIN_FILE, "$CMAKE_TOOLCHAIN_FILE")
set(VCPKG_INCLUDE_DIR, "$ENV{VCPKG_ROOT}/installed/x64-linux/include")

file(GLOB SOURCES "./src/**/*.cc")

find_package(cxxopts CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(Lua REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable("${PROJECT_NAME}"
  "./src/main.cc"
  ${SOURCES}
)

target_include_directories("${PROJECT_NAME}" PRIVATE
  ${LUA_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIR}
  "${VCPKG_INCLUDE_DIR}/sol"
  "${VCPKG_INCLUDE_DIR}/spdlog"
)
target_link_libraries("${PROJECT_NAME}" PRIVATE
  cxxopts::cxxopts
  freetype
  GLEW::GLEW
  ${LUA_LIBRARIES}
  ${SDL2_LIBRARY}
  SDL2::SDL2_image
  sol2::sol2
  spdlog::spdlog
  spdlog::spdlog_header_only
)
