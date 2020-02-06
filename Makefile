# OBJS: Specify the input .c/.cpp files to be compiled.
OBJS = main.cc modules/*.cc modules/**/*.cc utility/*.cc 

# CC: Specifiy the compiler to use.
CC = g++

# COMPILER_FLAGS: Set any additional compilation flags.
#   -w: Suppress warnings.
COMPILER_FLAGS = -w

# LINKER_FLAGS: Set any additional linker flags.
#   -lSDL2: Link the SDL2 library.
#   -lSDL2_image: Link the SDL2_image library.
#   -lSDL2_ttf: Link the SDL2_ttf library.
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

# OUT_NAME: Set the filename of the executable output.
OUT_NAME = term_engine

# Compile the source code here.
all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OUT_NAME)

