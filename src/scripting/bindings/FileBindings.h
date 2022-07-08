/// @author James Holtom

#ifndef FILE_BINDINGS_H
#define FILE_BINDINGS_H

#include "../../system/FileFunctions.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /// Binds the filesystem-related script functions to the Lua state.
  /**
   * @param[in] state The lua state to bind to.
   */
  void BindFileToState(sol::state& state)
  {
    state.create_named_table("fs",
      "read", &system::ReadFile,
      "write", &system::WriteFile,
      "exists", &system::FileExists,
      "getFiles", &system::GetFileList,
      "getFolders", &system::GetFolderList);
  }
}

#endif // ! FILE_BINDINGS_H
