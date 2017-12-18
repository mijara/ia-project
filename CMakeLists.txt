cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_C_FLAGS "-std=c99")

project (Tetris)

include_directories(include)

# Compile with a version that keeps lines in the board. Otherwise it will
# delete them.
add_definitions(-DKEEP_LINES)

# Apply optimizations for faster performance.
add_definitions(-DOPTIMIZATIONS)

# Meter the performance of the program.
# add_definitions(-DPERFORMANCE_METER)

# add_definitions(-DLATEX_OUTPUT)

set(SOURCE_FILES
    src/board.c
    src/tabu.c
    src/tabu_list.c
    src/features.c
    src/piece.c
    src/input.c
)

add_executable(Tetris src/main.c ${SOURCE_FILES})
set_property(TARGET Tetris PROPERTY C_STANDARD 99)
