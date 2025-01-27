#include "LogUtils.h"
#include "GLUtils.h"

namespace term_engine::utility {
  void InitLogger()
  {
    log_file.open(DEFAULT_LOG_LOCATION);
  }

  void ShutdownLogger()
  {
    log_file.close();
  }

  std::string GetFullFmt(const std::string& fmt, const std::string& type) {
    return "[" + type + "] " + fmt + "\n";
  }

  void LogKeyboardEvents(const SDL_Event& event)
  {
    if (event.key.repeat)
    {
      return;
    }

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

    LogDebug("Keyboard Event: {}, Key: {}, Modifiers: {}", keyboard_type, SDL_GetKeyName(event.key.keysym.sym), keyboard_modifiers);
  }

  void LogTextInputEvents(const SDL_Event& event)
  {
    if (event.type == SDL_TEXTEDITING)
    {
      LogDebug("Text Input Event [Composing]: Text: {}, Cursor Start: {}, Cursor Length: {}", event.edit.text, event.edit.start, event.edit.length);
    }
    else if (event.type == SDL_TEXTINPUT)
    {
      LogDebug("Text Input Event [Committed]: Text: {}", event.text.text);
    }
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
          LogDebug("Window ID: {}, Event: {}, X: {}, Y: {}", event.window.windowID, window_string, event.window.data1, event.window.data2);
        }
        else
        {
          LogDebug("Window ID: {}, Event: {}", event.window.windowID, window_string);
        }
      }
    }
  }

  void LogVAOData()
  {
    int vao_id, ebo_id, ebo_size, max_vertex_attribs, vbo_id;
    int vertex_attrib_is_enabled;

    LogDebug("Debugging currently bound VAO...");

    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao_id);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo_id);
    if (ebo_id > 0)
    {
      glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &ebo_size);
    }
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vertex_attribs);

    LogDebug("VAO ID: {}", vao_id);
    LogDebug("EBO ID: {}, size: {}", ebo_id, ebo_size);
    LogDebug("Max vertex attributes: {}", max_vertex_attribs);

    for (int i = 0; i < max_vertex_attribs; ++i) {
      glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &vertex_attrib_is_enabled);

      if (vertex_attrib_is_enabled)
      {
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vbo_id);

        LogDebug("Vertex attribute #{} is bound to VBO: {}", i, vbo_id);
      }
      else
      {
        LogDebug("Vertex attribute #{} is unbound.", i);
      }
    }
  }

  void LogVBOData()
  {
    int vbo_id, vbo_size;

    LogDebug("Debugging currently bound VBO...");

    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo_id);
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vbo_size);

    LogDebug("VBO ID: {}, size: {}", vbo_id, vbo_size);
  }
}
