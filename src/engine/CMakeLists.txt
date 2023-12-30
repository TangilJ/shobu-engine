cmake_minimum_required(VERSION 3.26)
project(ShobuEngine)

set(CMAKE_CXX_STANDARD 23)

add_library(
        ShobuEngine
        types.h
        movegen.cpp
        movegen.h
        print.cpp
        print.h
        consts.h
)
target_include_directories(ShobuEngine INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)