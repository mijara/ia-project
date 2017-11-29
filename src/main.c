#include "stdio.h"
#include "board.h"

int blocks_counter(struct board * board) {
    int count = 0;

    for (int y = board->height - 1; y >= 0; y--) {
        for (int x = 0; x < board->width; x++) {
            if (board_get(board, x, y) == BOARD_BLOCK_OCCUPIED) {
                count++;
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

    board_dump(board);

    printf("blocks_counter: %d\n", blocks_counter(board));

    board_free(&board);
}
