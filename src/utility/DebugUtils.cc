#include "DebugUtils.h"
#include "GLUtils.h"
#include "SpdlogUtils.h"
#include "../objects/BaseObject.h"

namespace term_engine::utility {
  DebugInfoList debug_info_list;

  std::string InfoItem::ToString() const
  {
    std::stringstream ss;
    const std::string time_str = std::to_string(total_time_);
    const uint32_t time_str_len = time_str.length() > 13 ? 13 : 16;

    ss
      << std::string(indent_, ' ')
      << std::setw(16 - indent_) << std::setiosflags(std::ios::left) << name_.substr(0, 16 - indent_)
      << std::setw(16) << std::resetiosflags(std::ios::left)
      << time_str.substr(0, time_str_len);

    if (time_str_len == 13)
    {
      ss << "...";
    }

    return ss.str();
  }
  
  void ObjectDebugInfo::AddSubItem(const std::string& name, uint32_t indent)
  {
    sub_list_.push_back(InfoItem(name, indent));
  }

  void ObjectDebugInfo::Start()
  {
    timer_.Start();
    main_item_.total_time_ = 0;
  }

  void ObjectDebugInfo::Interval(int interval_index)
  {
    if (sub_list_.size() == 0)
    {
      main_item_.total_time_ = timer_.GetIntervalElapsed();
    }
    else if (interval_index < sub_list_.size())
    {
      const uint64_t time = timer_.GetIntervalElapsed();

      sub_list_[interval_index].total_time_ = time;
      main_item_.total_time_ += time;
    }
    else
    {
      logger->warn("Invalid debug info index {} set.", interval_index);
    }
  }

  std::string ObjectDebugInfo::ToString() const
  {
    std::stringstream ss;

    ss << main_item_.ToString();

    for (const InfoItem& item : sub_list_)
    {
      ss << item.ToString();
    }

    return ss.str();
  }

  uint64_t ObjectDebugInfo::GetLineCount() const
  {
    return sub_list_.size() + 1;
  }

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

  ObjectDebugInfoPtr AddDebugInfo(const std::string& name)
  {
    ObjectDebugInfoPtr debug_info = ObjectDebugInfoPtr(new ObjectDebugInfo(name));
    debug_info_list.push_back(debug_info.get());

    return std::move(debug_info);
  }

  std::string GetDebugInfo(DebugSceneMenuOption option, uint64_t line)
  {
    const uint64_t info_end = std::min(line + 15, debug_info_list.size());
    std::stringstream return_string;

    for (uint64_t i = line; i < info_end; ++i)
    {
      return_string << debug_info_list[i]->ToString();
    }

    return return_string.str();
  }

  uint64_t GetDebugInfoCount()
  {
    return debug_info_list.size();
  }

  uint64_t GetDebugInfoLines()
  {
    uint64_t info_lines = 0;

    for (ObjectDebugInfo* debug_info : debug_info_list)
    {
      info_lines += debug_info->GetLineCount();
    }

    return info_lines;
  }
}
