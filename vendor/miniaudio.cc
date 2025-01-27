#define STB_VORBIS_HEADER_ONLY
#include "../vendor/stb_vorbis.c"

#define MA_NO_DEVICE_IO
#define MINIAUDIO_IMPLEMENTATION
#include "../vendor/miniaudio.h"

#undef STB_VORBIS_HEADER_ONLY
#include "../vendor/stb_vorbis.c"