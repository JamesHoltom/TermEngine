#include <iomanip>
#include <sstream>
#include "DebugUtils.h"
#include "GLUtils.h"
#include "SpdlogUtils.h"
#include "../objects/BaseObject.h"

namespace term_engine::utility {
  DebugInfoList debug_info_list;
  DebugSceneMenuOption menu_option;
  size_t current_line;
  
  void LogKeyboardEvents(const SDL_Event& event)
  {
    std::string keyboard_type;
    std::string keyboard_modifiers;

    switch (event.type) {
      case SDL_KEYDOWN: keyboard_type = "Pressed";  break;
      case SDL_KEYUP:   keyboard_type = "Released"; break;
      default:          return;
    }

    if (event.key.keysym.mod == KMOD_NONE)
    {
      keyboard_modifiers = "None";
    }
    else
    {
      if (event.key.keysym.mod & KMOD_ALT)
      {
        keyboard_modifiers += ", Alt";
      }

      if (event.key.keysym.mod & KMOD_CTRL)
      {
        keyboard_modifiers += ", Ctrl";
      }

      if (event.key.keysym.mod & KMOD_SHIFT)
      {
        keyboard_modifiers += ", Shift";
      }

      if (event.key.keysym.mod & KMOD_GUI)
      {
        keyboard_modifiers += ", Windows";
      }

      if (event.key.keysym.mod & KMOD_CAPS)
      {
        keyboard_modifiers += ", Caps Lock";
      }

      if (event.key.keysym.mod & KMOD_NUM)
      {
        keyboard_modifiers += ", Num Lock";
      }

      if (event.key.keysym.mod & KMOD_MODE)
      {
        keyboard_modifiers += ", AltGr";
      }

      keyboard_modifiers.erase(0, 2);
    }

    logger->debug("Keyboard Event: {}, Key: {}, Modifiers: {}", keyboard_type, SDL_GetKeyName(event.key.keysym.sym), keyboard_modifiers);
  }

  void LogWindowEvents(const SDL_Event& event)
  {
    if (event.type == SDL_WINDOWEVENT)
    {
      std::string window_string = "";
      bool showXY = false;

      switch (event.window.event)
      {
        case SDL_WINDOWEVENT_CLOSE:        window_string = "Closed";                      break;
        case SDL_WINDOWEVENT_ENTER:        window_string = "Mouse Entered";               break;
        case SDL_WINDOWEVENT_LEAVE:        window_string = "Mouse Left";                  break;
        case SDL_WINDOWEVENT_FOCUS_GAINED: window_string = "Focus Gained";                break;
        case SDL_WINDOWEVENT_FOCUS_LOST:   window_string = "Focus Lost";                  break;
        case SDL_WINDOWEVENT_EXPOSED:      window_string = "Exposed";                     break;
        case SDL_WINDOWEVENT_HIDDEN:       window_string = "Hidden";                      break;
        case SDL_WINDOWEVENT_SHOWN:        window_string = "Shown";                       break;
        case SDL_WINDOWEVENT_MAXIMIZED:    window_string = "Maximised";                   break;
        case SDL_WINDOWEVENT_MINIMIZED:    window_string = "Minimised";                   break;
        case SDL_WINDOWEVENT_RESTORED:     window_string = "Restored";                    break;
        case SDL_WINDOWEVENT_MOVED:        window_string = "Moved";        showXY = true; break;
        case SDL_WINDOWEVENT_RESIZED:      window_string = "Resized";      showXY = true; break;
        case SDL_WINDOWEVENT_SIZE_CHANGED: window_string = "Size Changed"; showXY = true; break;
        default:                           window_string = "Unknown";                     break;
      }

      if (window_string != "")
      {
        if (showXY)
        {
          logger->debug("Window ID: {}, Event: {}, X: {}, Y: {}", event.window.windowID, window_string, event.window.data1, event.window.data2);
        }
        else
        {
          logger->debug("Window ID: {}, Event: {}", event.window.windowID, window_string);
        }
      }
    }
  }

  void LogVAOData()
  {
    int vao_id, ebo_id, ebo_size, max_vertex_attribs, vbo_id;
    int vertex_attrib_is_enabled;

    logger->debug("Debugging currently bound VAO...");

    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao_id);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo_id);
    if (ebo_id > 0)
    {
      glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &ebo_size);
    }
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vertex_attribs);

    logger->debug("VAO ID: {}", vao_id);
    logger->debug("EBO ID: {}, size: {}", ebo_id, ebo_size);
    logger->debug("Max vertex attributes: {}", max_vertex_attribs);

    for (int i = 0; i < max_vertex_attribs; ++i) {
      glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &vertex_attrib_is_enabled);

      if (vertex_attrib_is_enabled)
      {
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vbo_id);

        logger->debug("Vertex attribute #{} is bound to VBO: {}", i, vbo_id);
      }
      else
      {
        logger->debug("Vertex attribute #{} is unbound.", i);
      }
    }
  }

  void LogVBOData()
  {
    int vbo_id, vbo_size;

    logger->debug("Debugging currently bound VBO...");

    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo_id);
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vbo_size);

    logger->debug("VBO ID: {}, size: {}", vbo_id, vbo_size);
  }

  ObjectDebugInfoWeakPtr AddDebugInfo(const std::string& name)
  {
    ObjectDebugInfoPtr debug_info = debug_info_list.emplace_back(std::make_shared<ObjectDebugInfo>(name));

    return debug_info;
  }

  void RemoveDebugInfo(const ObjectDebugInfoWeakPtr& ptr)
  {
    debug_info_list.erase(std::remove_if(debug_info_list.begin(), debug_info_list.end(), [&ptr](const ObjectDebugInfoPtr& info) { return ptr.lock()->name_ == info->name_; }), debug_info_list.end());
  }

  std::string GetDebugInfo()
  {
    size_t info_end = std::min(current_line + 15, debug_info_list.size());
    std::stringstream return_string;

    for (size_t i = current_line; i < info_end; ++i)
    {
      std::string timing = std::to_string(debug_info_list[i]->update_time_);
      std::string usage = std::to_string(debug_info_list[i]->ptr_uses_);

      return_string << std::setw(16) << std::setiosflags(std::ios::left) << debug_info_list[i]->name_;

      switch (menu_option)
      {
        case DebugSceneMenuOption::TIMING:
          return_string << std::setw(16) << std::resetiosflags(std::ios::left) << timing.substr(0, timing.length() > 13 ? 13 : 16);

          if (timing.length() > 13)
          {
            return_string << "...";
          }

          break;
        case DebugSceneMenuOption::PTR_USAGE:
          return_string << std::setw(16) << std::resetiosflags(std::ios::left) << usage.substr(0, usage.length() > 13 ? 13 : 16);

          if (usage.length() > 13)
          {
            return_string << "...";
          }

          break;
        case DebugSceneMenuOption::FLAGS:
          
          break;
      }
    }

    return return_string.str();
  }
}
