#include "stdio.h"
#include "board.h"
#include "tabu.h"
#include "string.h"

int landing_height(struct board * board) {
    // TODO: should optimize this...
    int height = board->height + 1;

    for (int x = 0; x < board->width; x++) {
        for (int y = 0; y < board->height; y++) {
            if (board_get(board, x, y) == BOARD_BLOCK_GHOST && y < height) {
                height = y;
            }
        }
    }

    return height;
}

int eroded_piece_cells(struct board * board) {
    int eroded_lines = 0;
    int eroded_cells = 0;

    for (int y = 0; y < board->height; y++) {
        int eroded = board_line_filled(board, y);

        eroded_lines += eroded;

        if (eroded) {
            for (int x = 0; x < board->width; x++) {
                eroded_cells += board_get(board, x, y) == BOARD_BLOCK_GHOST;
            }
        }
    }

    return eroded_lines * eroded_cells;
}

int holes(struct board * board) {
    int count = 0;

    for (int x = 0; x < board->width; x++) {
        for (int y = 0; y < board->height - 1; y++) {
            if (board_get(board, x, y) == BOARD_BLOCK_EMPTY) {
                if (board_get(board, x, y + 1) >= BOARD_BLOCK_OCCUPIED) {
                    count++;
                }
            }
        }
    }

    return count;
}

int movement(int state[], int buffer[], int len, int i) {
    if (i >= len) {
        return 0;
    }

    memcpy(buffer, state, len * sizeof(int));
    buffer[i] = !buffer[i];
    return 1;
}

int evaluate(int state[], int len) {
    int c = 0;
    c += state[0] * 5;
    c += state[1] * 10;
    c += state[2] * 8;
    c += state[3] * 15;
    c += state[4] * 2;
    return c;
}

int should_stop(int iterations) {
    return iterations > 10;
}

int main(int argc, char * argv[]) {
    int initial[5] = { 0, 0, 0, 0, 0};
    int solution[5];

    struct callbacks cbs;
    cbs.movement = movement;
    cbs.evaluate = evaluate;
    cbs.should_stop = should_stop;

    tabu(initial, solution, 10, cbs);

    printf("found best: %d\n", evaluate(solution, 5));

    /*
    struct board * board = board_new(5, 5);

    board_set(board, 0, 0, BOARD_BLOCK_OCCUPIED);
    board_set(board, 1, 0, BOARD_BLOCK_OCCUPIED);
    board_set(board, 2, 0, BOARD_BLOCK_OCCUPIED);
    board_set(board, 0, 1, BOARD_BLOCK_OCCUPIED);

    board_set(board, 4, 0, BOARD_BLOCK_OCCUPIED);
    board_set(board, 4, 1, BOARD_BLOCK_OCCUPIED);
    board_set(board, 4, 2, BOARD_BLOCK_OCCUPIED);
    board_set(board, 4, 3, BOARD_BLOCK_OCCUPIED);

    board_set(board, 1, 1, BOARD_BLOCK_GHOST);
    board_set(board, 2, 1, BOARD_BLOCK_GHOST);
    board_set(board, 3, 1, BOARD_BLOCK_GHOST);
    board_set(board, 3, 0, BOARD_BLOCK_GHOST);

    board_set(board, 2, 3, BOARD_BLOCK_OCCUPIED);
    board_set(board, 3, 3, BOARD_BLOCK_OCCUPIED);

    board_dump(board);

    printf("landing_height: %d\n", landing_height(board));
    printf("eroded_piece_cells: %d\n", eroded_piece_cells(board));
    printf("holes: %d\n", holes(board));

    board_free(&board);
    */
}
