#!/bin/bash
mkdir -p build
./clang-format-run.sh
g++ -o build/bin -DDISABLE_WIRING_PI $(pkg-config --cflags --libs gtk+-3.0) display/*.cpp display/epdif/*.cpp
exit $? # return if g++ worked
