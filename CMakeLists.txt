cmake_minimum_required(VERSION 3.17)
project(diff)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic -fsanitize=address")

set(SOURCE_EXE main.cpp)

set(SOURCE_LIB Diff.cpp RecDesc.cpp)

add_library(libdiff STATIC ${SOURCE_LIB})
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_EXE})

target_link_libraries(${CMAKE_PROJECT_NAME} libdiff)