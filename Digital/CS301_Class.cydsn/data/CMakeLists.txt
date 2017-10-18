file(GLOB DATA_FILES *.c *.h)

# add component
add_library(data ${DATA_FILES})

target_include_directories (data PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})