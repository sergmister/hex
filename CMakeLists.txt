cmake_minimum_required(VERSION 3.14)
project(hex VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE miowy_SRC src/*.cpp src/*.hpp)

add_executable(hex ${miowy_SRC})
