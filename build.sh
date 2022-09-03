#!/bin/bash

INCLUDES="-I/usr/include/SDL2 -I/usr/local/include -I./"
COMPILE_FLAGS="-std=c++2a -O0 -g"
LD_FLAGS="-lSDL2 -lGL -lGLEW"
#TOOLS="-fsanitize=address,undefined -fxray-instrument"

clang++ ${INCLUDES} ${COMPILE_FLAGS} ${TOOLS} -c Texture.cpp -o Texture.o
clang++ ${INCLUDES} ${COMPILE_FLAGS} ${LD_FLAGS} ${TOOLS} main.cpp Texture.o && ./a.out
