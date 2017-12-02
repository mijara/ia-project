#include "stdio.h"
#include "board.h"

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

int main(int argc, char * argv[]) {
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
}
