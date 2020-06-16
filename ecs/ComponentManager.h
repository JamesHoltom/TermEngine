#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

/*
  * Entt addresses an issue with error C4003 (i.e. Windows' min & max macros) with the following fix.
  * https://github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-C4003-the-min-the-max-and-the-macro
  */
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <entt/entt.hpp>

namespace term_engine::components {

}

#endif
