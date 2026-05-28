cmake_minimum_required(VERSION 3.21)

add_library(cisastor_testing STATIC
    functions.c
    testing_t.c
    shell.c
)

target_include_directories(cisastor_testing PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(cisastor::testing ALIAS cisastor_testing)

add_library(cisastor_testing_entry OBJECT
    testing.c
)

target_include_directories(cisastor_testing_entry PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(cisastor_testing_entry PRIVATE
    CISASTOR_LIBTESTING_PATH=\"$<TARGET_FILE:cisastor_testing>\"
)

add_executable(testing
    $<TARGET_OBJECTS:cisastor_testing_entry>
)

target_link_libraries(testing PRIVATE
    cisastor_testing
)

include(${PROJECT_SOURCE_DIR}/libs/testing/CisastorTesting.cmake)

cisastor_add_test(
	NAME testing_test
	SRCS functions_test.c shell_test.c
	IMPLS functions.c shell.c
)
