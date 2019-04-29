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
        ore/aes.h
        ore/crypto.h
        ore/crypto.c
        ore/errors.h
        ore/flags.h
        ore/ore.h
        ore/ore.c
        ore/ore_blk.h
        ore/ore_blk.c
        main.c)
