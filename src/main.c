#include "stdio.h"
#include "board.h"
#include "tabu.h"
#include "string.h"
#include "features.h"

int movement(int state[], int buffer[], int len, int i)
{
    if (i >= len) {
        return 0;
    }

    memcpy(buffer, state, len * sizeof(int));
    buffer[i] = !buffer[i];
    return 1;
}

int evaluate(int state[], int len)
{
    int c = 0;
    c += state[0] * 5;
    c += state[1] * 10;
    c += state[2] * 8;
    c += state[3] * 15;
    c += state[4] * 2;
    return c;
}

int stop(int iterations)
{
    return iterations > 10;
}

int main(int argc, char * argv[])
{
    // tabu search usage.
    int initial[5] = { 0, 0, 0, 0, 0 };
    int solution[5];

    struct tabu * tabu = tabu_new(evaluate, movement, stop);

    execute(tabu, initial, solution, 5, 3);

    printf("found best: %d\n", evaluate(solution, 5));

    /*
    // board usage.
    struct board * board = board_new(5, 5);

    board_set(board, 0, 0, BOARD_BLOCK_OCCUPIED);
    board_set(board, 1, 0, BOARD_BLOCK_OCCUPIED);
    board_set(board, 2, 0, BOARD_BLOCK_OCCUPIED);
    board_set(board, 0, 1, BOARD_BLOCK_OCCUPIED);

    board_set(board, 4, 0, BOARD_BLOCK_OCCUPIED);
    board_set(board, 4, 1, BOARD_BLOCK_OCCUPIED);
    board_set(board, 3, 2, BOARD_BLOCK_OCCUPIED);
    board_set(board, 4, 3, BOARD_BLOCK_OCCUPIED);

    board_set(board, 2, 1, BOARD_BLOCK_GHOST);
    board_set(board, 3, 1, BOARD_BLOCK_GHOST);
    board_set(board, 3, 0, BOARD_BLOCK_GHOST);

    board_set(board, 2, 3, BOARD_BLOCK_OCCUPIED);
    board_set(board, 3, 3, BOARD_BLOCK_OCCUPIED);

    printf("\n");
    board_dump(board);
    printf("\n");

    printf("landing_height:     %d\n", landing_height(board));
    printf("eroded_piece_cells: %d\n", eroded_piece_cells(board));
    printf("holes:              %d\n", holes(board));
    printf("row_transitions:    %d\n", row_transitions(board));
    printf("column_transitions: %d\n", column_transitions(board));
    printf("cumulative_wells:   %d\n", cumulative_wells(board));

    board_free(&board);
    */

    return 0;
}
