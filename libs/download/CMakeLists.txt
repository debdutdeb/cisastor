cmake_minimum_required(VERSION 3.21)

find_package(CURL REQUIRED)
add_library(cisastor_download STATIC download.c)
target_link_libraries(cisastor_download PRIVATE CURL::libcurl)
target_link_libraries(cisastor_download PRIVATE cisastor_http_client)
target_link_libraries(cisastor_download PRIVATE cisastor::utils)
target_include_directories(cisastor_download PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>
)

add_library(cisastor::download ALIAS cisastor_download)

