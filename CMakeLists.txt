cmake_minimum_required(VERSION 2.8.8)
project(code)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_library(src "src/bfs.c" "include/bfs.h" "src/edgelist.c" "include/edgelist.h" "src/sort.c" "include/sort.h" "src/timer.c" "include/timer.h" "src/vertex.c" "include/vertex.h")
add_subdirectory("datasets")

add_executable(main "src/main.c")
target_link_libraries(main src)