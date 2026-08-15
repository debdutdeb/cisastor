add_executable(progressbar progressbar.c)
target_link_libraries(progressbar PRIVATE cisastor::progressbar m)
