cmake_minimum_required(VERSION 3.13)
project(quadtree)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(src)

add_executable(Quadtree
        src/bounds.c
        src/node.c
        src/point.c
        src/quadtree.c
        src/quadtree.h
        test.c)
