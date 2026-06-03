cmake_minimum_required(VERSION 3.21)

add_library(cisastor_mover STATIC 
	mover.c
)

target_link_libraries(cisastor_mover PRIVATE cisastor::utils)
target_link_libraries(cisastor_mover PRIVATE cisastor::arena)

target_include_directories(cisastor_mover PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>
)

add_library(cisastor::mover ALIAS cisastor_mover)

include(${PROJECT_SOURCE_DIR}/libs/testing/CisastorTesting.cmake)

cisastor_add_test(
	NAME mover_test
	SRCS mover_test.c
	IMPLS mover.c
	LIBS cisastor::arena cisastor::utils
)
