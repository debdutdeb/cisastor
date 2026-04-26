add_library(cisastor_arena_allocator STATIC arena.c)
target_link_libraries(cisastor_arena_allocator PRIVATE cisastor::utils)
target_include_directories(cisastor_arena_allocator PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>
)
add_library(cisastor::arena ALIAS cisastor_arena_allocator)
