cmake_minimum_required(VERSION 3.21)

add_library(cisastor_json STATIC json.c)

add_executable(cisastor_json_generator generator/generator.c)
target_link_libraries(cisastor_json_generator PRIVATE cisastor::utils)

target_link_libraries(cisastor_json PRIVATE cisastor::utils)
target_link_libraries(cisastor_json PRIVATE cisastor::arena)

target_include_directories(cisastor_json PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>
)

add_library(cisastor::json ALIAS cisastor_json)



