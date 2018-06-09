cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
add_executable(example_union_find.exe example_union_find.cpp)


