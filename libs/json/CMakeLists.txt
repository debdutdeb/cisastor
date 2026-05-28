cmake_minimum_required(VERSION 3.21)

add_library(cisastor_json STATIC 
	lexer.c
	parser.c
)

add_executable(cisastor_json_generator generator/generator.c)
target_link_libraries(cisastor_json_generator PRIVATE cisastor::utils)
target_link_libraries(cisastor_json_generator PRIVATE cisastor::arena)
target_link_libraries(cisastor_json_generator PRIVATE cisastor::data::containers)

target_link_libraries(cisastor_json PRIVATE cisastor::utils)
target_link_libraries(cisastor_json PRIVATE cisastor::arena)
target_link_libraries(cisastor_json PRIVATE cisastor::data::containers)

target_include_directories(cisastor_json PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>
)

add_library(cisastor::json ALIAS cisastor_json)

include(${PROJECT_SOURCE_DIR}/libs/testing/CisastorTesting.cmake)

cisastor_add_test(
	NAME lexer_test
	SRCS lexer_test.c
	IMPLS lexer.c
	LIBS cisastor::arena cisastor::utils cisastor::data::containers
)



