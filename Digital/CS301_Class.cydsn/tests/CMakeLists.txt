cmake_minimum_required(VERSION 3.1)

#set project name
set(PRJ_NAME test_classes)

#libraries to link
set(LINK_LIBS "m")
#define cflags
set(CFLAGS "")
#define header include directories as list
set(INCLUDE_DIRS ../data ../algorithm ./)

set(DATA_HEADERS ../data/graph.h ../data/linked_list.h ../data/point.h ../data/priority_queue.h ../data/vector.h ../data/voidtypes.h)
set(ALGORITHM_HEADERS ../algorithm/graph_astar.h ../algorithm/graph_travel_all.h ../algorithm/pathfinder.h)

include_directories(INCLUDE_DIRS)

add_executable(test_graph test_graph.c ${DATA_HEADERS} ${ALGORITHM_HEADERS})
add_executable(test_linked_list test_linked_list.c ${DATA_HEADERS} ${ALGORITHM_HEADERS})
add_executable(test_priority_queue test_priority_queue.c ${DATA_HEADERS} ${ALGORITHM_HEADERS})
add_executable(test_vector test_vector.c ${DATA_HEADERS} ${ALGORITHM_HEADERS})

project(${PRJ_NAME})