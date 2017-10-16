set( TEST_LIBS algorithm data)

add_executable(test_graph test_graph.c)
add_executable(test_linked_list test_linked_list.c)
add_executable(test_priority_queue test_priority_queue.c)
add_executable(test_vector test_vector.c)

target_link_libraries(test_graph LINK_PUBLIC ${TEST_LIBS})
target_link_libraries(test_linked_list LINK_PUBLIC ${TEST_LIBS})
target_link_libraries(test_priority_queue LINK_PUBLIC ${TEST_LIBS})
target_link_libraries(test_vector LINK_PUBLIC ${TEST_LIBS})

