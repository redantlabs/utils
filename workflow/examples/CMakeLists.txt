cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

find_package(Boost REQUIRED COMPONENTS program_options)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIR})
  add_executable(example_workflow_with_options.exe example_workflow_with_options.cpp)
  target_link_libraries(example_workflow_with_options.exe ${Boost_LIBRARIES})
endif()
add_executable(example_workflow.exe example_workflow.cpp)


