cmake_minimum_required(VERSION 3.26)
project(Shobu)

set(CMAKE_CXX_STANDARD 23)


# SIMD code for NNUE makes use of AVX extension
add_definitions("-march=native")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions("-O0")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions("-Ofast")
else ()
    message(FATAL_ERROR "CMAKE_BUILD_TYPE was not 'Debug' or 'Release'")
endif ()

set(
        ENGINE_SOURCES

        src/engine/types.h
        src/engine/movegen.cpp
        src/engine/movegen.h
        src/engine/print.cpp
        src/engine/print.h
        src/engine/consts.h
        src/engine/perft.cpp
        src/engine/perft.h
)


add_library(
        ShobuEngine
        ${ENGINE_SOURCES}
)
target_include_directories(ShobuEngine INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)


add_executable(
        Shobu
        src/main.cpp
)
target_link_libraries(Shobu ShobuEngine)


# Tests only run on Debug mode
add_subdirectory(extern/doctest)
add_executable(
        ShobuTests
        test/passivemovetest.cpp
        test/aggressivemovetest.cpp
        test/movegentest.cpp

        ${ENGINE_SOURCES}
        # Adding engine sources directly because required template functions are
        # not compiled in a library
)
target_link_libraries(ShobuTests doctest)


add_subdirectory(extern/pybind11)
pybind11_add_module(engine src/bindings.cpp)
target_link_libraries(engine PRIVATE ShobuEngine)