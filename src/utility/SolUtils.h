/// @author James Holtom

#ifndef SOL_UTILS_H
#define SOL_UTILS_H

/*
  * Sol currently has an issue regarding deprecation error C4996.
  * https://github.com/ThePhD/sol2/issues/945
  *
  * After MSVC 16.6.0, there is an issue building Sol.
  */
#pragma warning(push)
#pragma warning(disable: 4996)

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#pragma warning(pop)

#endif // ! SOL_UTILS_H
