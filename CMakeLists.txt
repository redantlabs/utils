project(utils)

cmake_minimum_required(VERSION 2.6)

# list packages
file(GLOB CONFIGURED_PACKAGES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/" "${CMAKE_CURRENT_SOURCE_DIR}/*")

# install
foreach(dir ${CONFIGURED_PACKAGES})
  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/include) 
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/include DESTINATION . COMPONENT devel)
  endif()
endforeach()

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake)
