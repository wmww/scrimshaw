
cmake_minimum_required(VERSION 3.1)

project(Scrimshaw)

file(GLOB scrimshaw_sources
     util/*.cpp
     display/*.cpp)

set(scrimshaw_libs
    -lpthread)

file(GLOB hedgehog_sources
     main/hedgehog_main.cpp
     hedgehog/*.cpp
     hedgehog/main/*.cpp
     hedgehog/scene/*.cpp
     hedgehog/backend/*.cpp
     hedgehog/wayland/*.cpp
     hedgehog/wayland/protocols/*.c)

set(hedgehog_libs
    wayland-server
    xkbcommon
    udev
    input)

file(GLOB gtk_display_sources
     display/gtk/*.cpp)

file(GLOB epdif_display_sources
     display/epdif/*.cpp)

set(epdif_libs
    -lwiringPi)

file(GLOB display_test_sources
     main/display_test_main.cpp)

include_directories(".")
include_directories("util")

find_package(PkgConfig REQUIRED)
pkg_check_modules(gtk3 gtk+-3.0)

if(gtk3_FOUND)
    message("GTK found")
    # Setup CMake to use GTK+, tell the compiler where to look for headers
    # and to the linker where to look for libraries
    include_directories(${GTK3_INCLUDE_DIRS})
    link_directories(${GTK3_LIBRARY_DIRS})
    # Add other flags to the compiler
    add_definitions(${GTK3_CFLAGS_OTHER})

    add_executable(test_gtk ${gtk_display_sources} ${display_test_sources} ${scrimshaw_sources})
    target_link_libraries(test_gtk
                          ${gtk3_LIBRARIES}
                          ${scrimshaw_libs})

    add_executable(hedgehog_gtk ${gtk_display_sources} ${hedgehog_sources} ${scrimshaw_sources})
    target_link_libraries(hedgehog_gtk
                          ${gtk3_LIBRARIES}
                          ${hedgehog_libs}
                          ${scrimshaw_libs})

else()
    message("GTK not found")
endif()

find_library(WIRING_PI_LIB wiringPi)

if (WIRING_PI_LIB)
    message("Wiring Pi found")
    add_executable(test_epdif ${epdif_display_sources} ${display_test_sources} ${scrimshaw_sources})
    target_link_libraries(test_gtk
                          ${epdif_libs}
                          ${scrimshaw_libs})

    add_executable(hedgehog_epdif ${epdif_display_sources} ${hedgehog_sources} ${scrimshaw_sources})
    target_link_libraries(hedgehog_gtk
                          ${epdif_libs}
                          ${hedgehog_libs}
                          ${scrimshaw_libs})
else()
    message("Wiring Pi not found")
endif()
