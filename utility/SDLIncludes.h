/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef UTIL_SDL_INCLUDES_H
#define UTIL_SDL_INCLUDES_H

#ifdef _MSC_VER
#include <SDL.h>
#include <SDL_ttf.h>
#elif __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

#endif // ! UTIL_SDL_INCLUDES_H
