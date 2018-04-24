#!/bin/bash
find . -iname *.h -o -iname *.cpp -o -iname *.c | xargs clang-format -i
