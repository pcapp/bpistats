cmake_minimum_required(VERSION 3.11)
project(scratch)

set(CMAKE_CXX_STANDARD 17)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(${SRC_DIR})
add_subdirectory(test)