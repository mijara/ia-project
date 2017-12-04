#include "stdio.h"
#include "board.h"
#include "tabu.h"
#include "string.h"
#include "features.h"
#include "tabu_list.h"

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
    int state1[5] = { 1, 0, 0, 0, 0 };
    int state2[5] = { 0, 1, 0, 0, 0 };
    int state3[5] = { 0, 0, 1, 0, 0 };
    int state4[5] = { 0, 0, 0, 1, 0 };
    int state5[5] = { 0, 0, 0, 1, 1 };

    struct tabu_list * list = tabu_list_new(3, 5);
    tabu_list_insert(list, state1);
    tabu_list_insert(list, state2);
    tabu_list_insert(list, state3);
    tabu_list_insert(list, state4);
    tabu_list_insert(list, state5);

    printf("1 %d\n", tabu_list_contains(list, state1));
    printf("2 %d\n", tabu_list_contains(list, state2));
    printf("3 %d\n", tabu_list_contains(list, state3));
    printf("4 %d\n", tabu_list_contains(list, state4));
    printf("5 %d\n", tabu_list_contains(list, state5));

    tabu_list_free(&list);

    /*
    int initial[5] = { 0, 0, 0, 0, 0 };
    int solution[5];

    struct callbacks cbs;
    cbs.movement = movement;
    cbs.evaluate = evaluate;
    cbs.should_stop = should_stop;

    tabu(initial, solution, 10, cbs, 3);

    printf("found best: %d\n", evaluate(solution, 5));
    */

    /*
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
