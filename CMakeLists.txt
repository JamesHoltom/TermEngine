cmake_minimum_required(VERSION "3.16")

project("TermEngine" VERSION "0.2")

file(GLOB SOURCES "src/**/*.cc")

add_executable("${PROJECT_NAME}" ${SOURCES})

find_package(freetype CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
target_link_libraries("${PROJECT_NAME}" PRIVATE freetype GLEW::GLEW SDL2::SDL2main SDL2::SDL2-static SDL2::SDL2_image spdlog::spdlog spdlog::spdlog_header_only sol2::sol2)

