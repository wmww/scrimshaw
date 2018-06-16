
cmake_minimum_required(VERSION 3.1)

project(Scrimshaw)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

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
     display/epdif/*.cpp
     gpio/*.cpp
     emulator/*.cpp)

file(GLOB display_test_sources
     main/display_test_main.cpp)

include_directories(".")

find_library(WIRING_PI_LIB wiringPi)

if (WIRING_PI_LIB)
    message("Wiring Pi found")
    set(epdif_libs
        -lwiringPi)
else()
    message("Wiring Pi not found")
    add_definitions(-DEMULATED_WIRING_PI)
	set(epdif_libs)
endif()

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
                          ${scrimshaw_libs}
                          ${gtk3_LIBRARIES})

    add_executable(hedgehog_gtk ${gtk_display_sources} ${hedgehog_sources} ${scrimshaw_sources})
    target_link_libraries(hedgehog_gtk
                          ${hedgehog_libs}
                          ${scrimshaw_libs}
                          ${gtk3_LIBRARIES})

    if (WIRING_PI_LIB)
    else()
        add_definitions(-DEMULATED_WIRING_PI)
        set(scrimshaw_libs ${scrimshaw_libs}
            ${gtk3_LIBRARIES})
        set(scrimshaw_sources ${scrimshaw_sources}
            ${gtk_display_sources})
    endif()
else()
    message("GTK not found")
    add_definitions(-DDEFAULT_DISPLAY_EPDIF)
endif()

add_executable(test_epdif ${epdif_sources} ${display_test_sources} ${scrimshaw_sources})
target_link_libraries(test_epdif
                      ${epdif_libs}
                      ${scrimshaw_libs})

add_executable(hedgehog_epdif
               ${epdif_sources}
               ${hedgehog_sources}
               ${scrimshaw_sources})
target_link_libraries(hedgehog_epdif
                      ${epdif_libs}
                      ${hedgehog_libs}
                      ${scrimshaw_libs})
