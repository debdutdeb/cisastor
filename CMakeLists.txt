cmake_minimum_required(VERSION 3.21)
project(cisastor VERSION 0.1.0 LANGUAGES C CXX)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(libs/utils)
add_subdirectory(libs/testing)
add_subdirectory(libs/arena_allocator)
add_subdirectory(libs/data)

add_subdirectory(libs/progressbar)
add_subdirectory(libs/json)
add_subdirectory(examples)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()
