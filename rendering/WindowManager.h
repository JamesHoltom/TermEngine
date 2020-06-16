/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <memory>
#include <string>
#include <unordered_map>

#include "Window.h"

namespace term_engine::window_management {
  typedef std::shared_ptr<Window> WindowPtr;
  typedef std::unordered_map<std::string, WindowPtr> WindowMap;
  typedef std::pair<std::string, WindowPtr> WindowPair;

  void InitDefaultWindow();
  void CleanUpWindows();

  WindowPtr GetDefaultWindow();
  WindowPtr GetWindow(const std::string& name);
  int GetWindowCount();

  WindowPtr AddWindow(const std::string& name);
  void RemoveWindow(const std::string& name);

  void LogWindowEvents(SDL_Event& event);

  void Logic(SDL_Event& event);
  void Render();

  extern WindowMap window_list;
}

#endif
