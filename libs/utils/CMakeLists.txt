cmake_minimum_required(VERSION 3.21)

add_library(cisastor_utils INTERFACE)

target_include_directories(cisastor_utils INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

add_library(cisastor::utils ALIAS cisastor_utils)
