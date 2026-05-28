cmake_minimum_required(VERSION 3.21)

add_library(cisastor_data_containers STATIC
	containers/array_list.c
	containers/string_view.c
	containers/iterator.c
	containers/string.c
	containers/string_builder.c
)

target_include_directories(cisastor_data_containers INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

add_library(cisastor::data::containers ALIAS cisastor_data_containers)

target_link_libraries(cisastor_data_containers PRIVATE cisastor::arena)
target_link_libraries(cisastor_data_containers PRIVATE cisastor::utils)
target_link_libraries(cisastor_data_containers PRIVATE cisastor::testing)

include(${PROJECT_SOURCE_DIR}/libs/testing/CisastorTesting.cmake)

cisastor_add_test(
	NAME array_list_test
	SRCS containers/array_list_test.c
	IMPLS containers/array_list.c
	LIBS cisastor::arena cisastor::utils
)

cisastor_add_test(
	NAME iterator_test
	SRCS containers/iterator_test.c
	IMPLS containers/iterator.c containers/array_list.c
	LIBS cisastor::arena cisastor::utils
)

cisastor_add_test(
	NAME string_view_test
	SRCS containers/string_view_test.c
	IMPLS containers/string_view.c
	LIBS cisastor::arena cisastor::utils
)

cisastor_add_test(
	NAME string_test
	SRCS containers/string_test.c
	IMPLS containers/string.c containers/array_list.c containers/iterator.c
	LIBS cisastor::arena cisastor::utils
)

cisastor_add_test(
	NAME string_builder_test
	SRCS containers/string_builder_test.c
	IMPLS containers/string_builder.c containers/string.c containers/array_list.c containers/iterator.c
	LIBS cisastor::arena cisastor::utils
)
