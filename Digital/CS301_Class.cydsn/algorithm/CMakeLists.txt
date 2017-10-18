file(GLOB ALGORITHM_FILES *.c *.h)

# add component
add_library(algorithm ${ALGORITHM_FILES})

target_link_libraries(algorithm data)

target_include_directories (algorithm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})