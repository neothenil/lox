cmake_minimum_required(VERSION 3.31)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(AUTOGEN_DIR ${CMAKE_SOURCE_DIR}/src/autogen)
execute_process(
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/generate_ast.py .
    WORKING_DIRECTORY ${AUTOGEN_DIR}
    COMMAND_ERROR_IS_FATAL ANY
)

project(lox CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(vendors/fmt)

include_directories(src vendors/magic_enum/include)
file(GLOB sources ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(lox ${sources})
target_link_libraries(lox fmt::fmt)
