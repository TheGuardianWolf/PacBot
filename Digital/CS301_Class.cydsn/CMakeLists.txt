cmake_minimum_required(VERSION 3.1)

#set project name
project(test_classes)

#define cflags
set(CFLAGS "")

add_subdirectory(data)
add_subdirectory(algorithm)

set( TEST_LIBS algorithm data)
set( TEST_HEADERS "tests/minunit.h" )

add_executable(test_graph tests/test_graph.c ${TEST_HEADERS})
add_executable(test_linked_list tests/test_linked_list.c ${TEST_HEADERS})
add_executable(test_priority_queue tests/test_priority_queue.c ${TEST_HEADERS})
add_executable(test_vector tests/test_vector.c ${TEST_HEADERS})

target_link_libraries(test_graph ${TEST_LIBS})
target_link_libraries(test_linked_list ${TEST_LIBS})
target_link_libraries(test_priority_queue ${TEST_LIBS})
target_link_libraries(test_vector ${TEST_LIBS})

