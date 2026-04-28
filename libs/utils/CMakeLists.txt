cmake_minimum_required(VERSION 3.21)

add_library(cisastor_utils STATIC
	array_list.c
	string_view.c
)

target_include_directories(cisastor_utils INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

add_library(cisastor::utils ALIAS cisastor_utils)

target_link_libraries(cisastor_utils PRIVATE cisastor::arena)
