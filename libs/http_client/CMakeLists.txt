cmake_minimum_required(VERSION 3.21)

find_package(CURL REQUIRED)

add_library(cisastor_http_client STATIC client.c)
target_link_libraries(cisastor_http_client PRIVATE CURL::libcurl)
target_include_directories(cisastor_http_client PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>
)

add_library(cisastor::http_client ALIAS cisastor_http_client)


