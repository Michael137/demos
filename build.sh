#!/bin/bash

#clang++ main.cpp -std=c++2a -O0 -g -lSDL2 -lGL -lGLEW -fsanitize=address,undefined -fxray-instrument && ./a.out

clang++ main.cpp -std=c++2a -O0 -g -lSDL2 -lGL -lGLEW -fxray-instrument -fxray-instruction-threshold=1 -fxray-always-instrument=always-instrument.txt

XRAY_OPTIONS="patch_premain=true" ./a.out
