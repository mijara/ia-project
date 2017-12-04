cmake_minimum_required(VERSION 2.8.11)

project (Tetris)

include_directories(include)

set(SOURCE_FILES
    src/board.c
    src/tabu.c
    src/tabu_list.c
)

add_executable(Tetris src/main.c ${SOURCE_FILES})
