/// @author James Holtom

#ifndef FILE_BINDINGS_H
#define FILE_BINDINGS_H

#include "../../system/FileFunctions.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /// Binds the filesystem-related script functions to the Lua state.
  /**
   * @params[in] state The lua state to bind to.
   */
  void BindFileToState(sol::state& state)
  {
    state.create_named_table("fs",
      "getFileList", [&](const std::string& directory) { return system::GetFileList(File::Relative(std::filesystem::path(directory))); },
      "getFolderList", [&](const std::string& directory) { return system::GetFolderList(File::Relative(std::filesystem::path(directory))); });
  }
}

#endif // ! FILE_BINDINGS_H
