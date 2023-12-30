cmake_minimum_required(VERSION 3.26)
project(Shobu)

set(CMAKE_CXX_STANDARD 23)


# SIMD code for NNUE makes use of AVX extension
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions("-mavx -O0")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions("-mavx -Ofast")
else ()
    message(FATAL_ERROR "CMAKE_BUILD_TYPE was not 'Debug' or 'Release'")
endif ()


add_executable(
        Shobu
        src/main.cpp
        src/types.h
        src/movegen.cpp
        src/movegen.h
        src/print.cpp
        src/print.h
        src/consts.h
)
