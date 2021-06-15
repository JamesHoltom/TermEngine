# CC: Specify the compiler to use.
CC = g++

# COMPILER_FLAGS: Set any additional compilation flags.
COMPILER_FLAGS = -std=c++17
# Debug-only flags:
debug: COMPILER_FLAGS += -Wall -Wextra
# Release-only flags:
release: COMPILER_FLAGS += -W3

# LINKER_FLAGS: Set any additional linker flags.
LINKER_FLAGS =  -lSDL2 -lSDL2_image -lGLEW -lGLU -lGL -llua

# OUT_DIR: Set the output directory of the executable.
debug: OUT_DIR = x64/Debug
release: OUT_DIR = x64/Release

# OUT_NAME: Set the filename of the executable output.
OUT_NAME = term_engine

.PHONY: clean
clean:
	rm -f $(OUT_DIR)/*
