#include "features.h"

int landing_height(struct board * board)
{
    int height = board->height + 1;

    for (int x = 0; x < board->width; x++) {
        for (int y = 0; y < board->height; y++) {
            if (board_get(board, x, y) == BOARD_BLOCK_GHOST && y < height) {
                height = y;
            }
        }
    }
    
    // NOTE: the return value when there's no GHOST pieces will be
    // height + 1.

    return height;
}

int eroded_piece_cells(struct board * board)
{
    int eroded_lines = 0;
    int eroded_cells = 0;

    for (int y = 0; y < board->height; y++) {
        int eroded = board_is_row_filled(board, y);

        eroded_lines += eroded;

        if (eroded) {
            for (int x = 0; x < board->width; x++) {
                eroded_cells += board_get(board, x, y) == BOARD_BLOCK_GHOST;
            }
        }
    }

    return eroded_lines * eroded_cells;
}

int holes(struct board * board)
{
    int count = 0;

    for (int x = 0; x < board->width; x++) {
        for (int y = 0; y < board->height - 1; y++) {
            if (!board_is_occupied(board, x, y)) {
                if (board_is_occupied(board, x, y + 1)) {
                    count++;
                }
            }
        }
    }

    return count;
}

int row_transitions(struct board * board)
{
    int sum = 0;

    for (int j = 0; j < board->height; j++) {
        // the left outside of the board is considered as occupied.
        int state = BOARD_BLOCK_OCCUPIED;

        for (int i = 0; i < board->width; i++) {
            int cell = board_is_occupied(board, i, j);
            if (cell != state) {
                sum++;
            }
            state = cell;
        }

        // the right outside of the board is considered as occupied.
        if (state != BOARD_BLOCK_OCCUPIED) {
            sum++;
        }
    }

    return sum;
}

int column_transitions(struct board * board)
{
    int sum = 0;

    for (int i = 0; i < board->width; i++) {
        // the bottom outside of the board is considered as occupied.
        int state = BOARD_BLOCK_OCCUPIED;

        for (int j = 0; j < board->height; j++) {
            int cell = board_is_occupied(board, i, j);

            if (cell != state) {
                sum++;
            }

            state = cell;
        }
    }

    return sum;
}

int _arithmetic_sum(int n)
{
    return n < 0 ? 0 : n * (n + 1) / 2;
}

int cumulative_wells(struct board * board)
{
    int sum = 0;

    // check the left-most column.
    int diff_left = board_column_height(board, 1) - board_column_height(board, 0);
    sum += _arithmetic_sum(diff_left);

    // check the middle of the board.
    for (int x = 1; x < board->width - 1; x++) {
        int height = board_column_height(board, x);

        int diff_left = board_column_height(board, x - 1) - height;
        int diff_right = board_column_height(board, x + 1) - height;

        int minimum = diff_left < diff_right ? diff_left : diff_right;
        sum += _arithmetic_sum(minimum);
    }

    // check the right-most column.
    int diff_right = board_column_height(board, board->width - 2) - 
                        board_column_height(board, board->width - 1);
    sum += _arithmetic_sum(diff_right);

    return sum;
}
