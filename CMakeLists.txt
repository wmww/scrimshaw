
cmake_minimum_required(VERSION 3.0)

project(Scrimshaw)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})

file(GLOB
     SOURCES
     util/*.cpp
     display/*.cpp
     display/epdif/*.cpp
     hedgehog/main/*.cpp
     hedgehog/scene/*.cpp
     hedgehog/backend/*.cpp
     hedgehog/wayland/*.cpp
     hedgehog/wayland/protocols/*.c
     )

include_directories("util")

add_executable(display_test ${SOURCES})

target_compile_definitions(display_test PRIVATE DISABLE_WIRING_PI)

# Link the target to the GTK+ libraries
target_link_libraries(
    display_test
    ${GTK3_LIBRARIES}
    -lpthread
    wayland-server
    xkbcommon
    )
