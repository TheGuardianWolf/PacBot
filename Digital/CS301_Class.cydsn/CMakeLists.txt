cmake_minimum_required(VERSION 3.1)

#set project name
project(test_classes)

#define cflags
set(CFLAGS "")

add_subdirectory(data)
add_subdirectory(algorithm)
add_subdirectory(tests)
