ALL_OBJS = **/*.cc
THIRDPARTY_OBJS = thirdparty/**/*.cc

# OBJS: Specify the input .c/.cpp files to be compiled.
OBJS = main.cc $(filter-out $(THIRDPARTY_OBJS), $(ALL_OBJS))

# INCS: Specify directories to include when compiling.
INCS = thirdparty

# CC: Specifiy the compiler to use.
CC = g++

# COMPILER_FLAGS: Set any additional compilation flags.
#   -Wall: Enable all warnings.
#   -Wextra: Enable additional warnings, not covered by -Wall.
#   -std: Set the C++ Standard to compile to.
COMPILER_FLAGS = -Wall -Wextra -std=c++17

# LINKER_FLAGS: Set any additional linker flags.
#   -lSDL2: Link the SDL2 library.
#   -lSDL2_mixer: Link the SDL2_mixer library.
#   -lSDL2_ttf: Link the SDL2_ttf library.
#   -lGLEW: Link the GL Extension Wrangler library.
#   -lGLU: Link the GL Utilities library.
#   -lGL: Link the OpenGL library.
#   -llua: Link Lua.
LINKER_FLAGS =  -lSDL2 -lSDL2_mixer -lSDL2_ttf -lGLEW -lGLU -lGL -llua

# OUT_DIR: Set the output directory of the executable.
OUT_DIR = Debug/x64

# OUT_NAME: Set the filename of the executable output.
OUT_NAME = term_engine

# Compile the source code here.
all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -I $(INCS) $(LINKER_FLAGS) -o $(OUT_DIR)/$(OUT_NAME)
