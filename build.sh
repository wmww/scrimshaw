#!/bin/bash
mkdir -p build
./clang-format-run.sh
g++ -Wall -g -o build/bin -DDISABLE_WIRING_PI $(pkg-config --cflags gtk+-3.0) display/*.cpp display/epdif/*.cpp $(pkg-config --libs gtk+-3.0)
exit $? # return if g++ worked
