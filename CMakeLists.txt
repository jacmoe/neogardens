#
#   This file is part of the
# ███╗   ██╗███████╗ ██████╗  ██████╗  █████╗ ██████╗ ██████╗ ███████╗███╗   ██╗███████╗
# ████╗  ██║██╔════╝██╔═══██╗██╔════╝ ██╔══██╗██╔══██╗██╔══██╗██╔════╝████╗  ██║██╔════╝
# ██╔██╗ ██║█████╗  ██║   ██║██║  ███╗███████║██████╔╝██║  ██║█████╗  ██╔██╗ ██║███████╗
# ██║╚██╗██║██╔══╝  ██║   ██║██║   ██║██╔══██║██╔══██╗██║  ██║██╔══╝  ██║╚██╗██║╚════██║
# ██║ ╚████║███████╗╚██████╔╝╚██████╔╝██║  ██║██║  ██║██████╔╝███████╗██║ ╚████║███████║
# ╚═╝  ╚═══╝╚══════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═══╝╚══════╝
#                                                                                       
#   project : https://github.com/jacmoe/neogardens
#
#   Copyright 2017 Jacob Moen
#
#
cmake_minimum_required(VERSION 2.8.12)
project(neogardens)

include(CMakeToolsHelpers OPTIONAL)

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/memsharded/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(REQUIRES nasl/0.4@jacmoe/testing
                BASIC_SETUP
                BUILD missing)

#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#conan_basic_setup()

find_package(PkgConfig REQUIRED)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image)

find_package(OpenGL REQUIRED)

find_package(GLEW 1.10)
if(${GLEW_FOUND})
  add_definitions(-DUSE_GLEW)
endif()

include_directories(
    ${CONAN_INCLUDE_DIRS_NASL}/include
    ${CONAN_INCLUDE_DIRS_NASL}/3rd/include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2IMAGE_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

add_subdirectory(1pal)
add_subdirectory(2maze)
add_subdirectory(3move)
add_subdirectory(4rays)
add_subdirectory(5rayt)
add_subdirectory(6rayf)
add_subdirectory(7rayl)
