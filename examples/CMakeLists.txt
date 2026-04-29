add_executable(progressbar progressbar.c)
target_link_libraries(progressbar PRIVATE cisastor::progressbar m)
add_executable(array_list array_list.c)
target_link_libraries(array_list PRIVATE cisastor::utils)
target_link_libraries(array_list PRIVATE cisastor::data::containers)
target_link_libraries(array_list PRIVATE cisastor::arena)

add_executable(json_parse json.c)
target_link_libraries(json_parse PRIVATE cisastor::utils)
target_link_libraries(json_parse PRIVATE cisastor::data::containers)
target_link_libraries(json_parse PRIVATE cisastor::json)
target_link_libraries(json_parse PRIVATE cisastor::arena)

