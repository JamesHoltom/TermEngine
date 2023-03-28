#include "Application.h"
#include "system/CLArguments.h"

/**
 * @brief The entrypoint of the program.
 * 
 * @param[in] argc The amount of arguments passed to the program on startup.
 * @param[in] argv An array pointer to a list of string pointers. The first element is always the program name.
 * @returns A status code indicating how the program ended.
 */
int main(int argc, char** argv) {
  term_engine::system::GetCLArguments(argc, argv);
  
  term_engine::Init();

  term_engine::Run();

  term_engine::CleanUp();

  return 0;
}
