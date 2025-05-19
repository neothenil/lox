cmake_minimum_required(VERSION 3.31)
project(lox CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
file(GLOB sources ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(lox ${sources})
