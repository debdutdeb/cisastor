cmake_minimum_required(VERSION 3.21)

add_library(cisastor_data_containers STATIC
	containers/array_list.c
	containers/string_view.c
)

target_include_directories(cisastor_data_containers INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

add_library(cisastor::data::containers ALIAS cisastor_data_containers)

target_link_libraries(cisastor_data_containers PRIVATE cisastor::arena)
target_link_libraries(cisastor_data_containers PRIVATE cisastor::utils)

