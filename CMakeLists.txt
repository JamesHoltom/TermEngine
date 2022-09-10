cmake_minimum_required(VERSION 3.16.0)

project("TermEngine" VERSION 0.6.0 LANGUAGES CXX C)

#include(CTest)
#enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_EXE_LINKER_FLAGS, "-lpthread -lm -ldl")
set(CMAKE_TOOLCHAIN_FILE, "$CMAKE_TOOLCHAIN_FILE")
set(VCPKG_INCLUDE_DIR, "$ENV{VCPKG_ROOT}/installed/x64-linux/include")

file(GLOB SOURCES "./src/**/*.cc")

find_package(cxxopts CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
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
