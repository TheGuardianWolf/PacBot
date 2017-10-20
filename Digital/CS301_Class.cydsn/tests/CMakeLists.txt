set( TEST_LIBS algorithm data m )

add_executable(test_graph test_graph.c)
add_executable(test_linked_list test_linked_list.c)
add_executable(test_priority_queue test_priority_queue.c)
add_executable(test_vector test_vector.c)
add_executable(test_graph_astar test_graph_astar.c)
add_executable(test_graph_travel_all test_graph_travel_all.c)
add_executable(test_pathfinder test_pathfinder.c)

target_link_libraries(test_graph LINK_PUBLIC ${TEST_LIBS})
target_link_libraries(test_linked_list LINK_PUBLIC ${TEST_LIBS})
target_link_libraries(test_priority_queue LINK_PUBLIC ${TEST_LIBS})
target_link_libraries(test_vector LINK_PUBLIC ${TEST_LIBS})
target_link_libraries(test_graph_astar LINK_PUBLIC ${TEST_LIBS})
target_link_libraries(test_graph_travel_all LINK_PUBLIC ${TEST_LIBS})
target_link_libraries(test_pathfinder LINK_PUBLIC ${TEST_LIBS})

