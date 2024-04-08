cmake_minimum_required(VERSION 3.26)
project(Shobu)

set(CMAKE_CXX_STANDARD 23)


# SIMD code for NNUE makes use of AVX extension
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
        src/engine/search.cpp
        src/engine/search.h
)

add_executable(
        Shobu
        src/main.cpp

        ${ENGINE_SOURCES}
)


# Tests only run on Debug mode
add_subdirectory(extern/doctest)
add_executable(
        ShobuTests
        test/passivemovetest.cpp
        test/aggressivemovetest.cpp
        test/movegentest.cpp

        ${ENGINE_SOURCES}
)
target_compile_definitions(ShobuTests PRIVATE STANDALONE_TESTS)
target_link_libraries(ShobuTests doctest)