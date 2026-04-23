cmake_minimum_required(VERSION 3.21)
project(cisastor VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(libs/progressbar)
add_subdirectory(examples)

