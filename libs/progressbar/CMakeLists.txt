add_library(cisastor_progressbar STATIC
    progressbar.c
)

target_include_directories(cisastor_progressbar PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

add_library(cisastor::progressbar ALIAS cisastor_progressbar)
