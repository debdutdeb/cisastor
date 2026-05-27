add_library(cisastor_testing INTERFACE)

target_include_directories(cisastor_testing INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

add_library(cisastor::testing ALIAS cisastor_testing)
