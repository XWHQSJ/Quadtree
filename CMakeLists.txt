cmake_minimum_required(VERSION 3.12)
project(Quadtree C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(Quadtree
        src/bounds.c
        src/node.c
        src/point.c
        src/quadtree.c
        src/quadtree.h
        main.c)
