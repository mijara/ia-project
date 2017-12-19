#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "board.h"
#include "tabu.h"
#include "piece.h"
#include "input.h"

#include "features.c"

#define DEBUG

struct input * input;

int evaluate_board(struct board * board, int state[], int len) {
    int sum = 0;
    sum += state[0] * landing_height(board);
    sum += state[1] * eroded_piece_cells(board);
    sum += state[2] * holes(board);
    sum += state[3] * row_transitions(board);
    sum += state[4] * column_transitions(board);
    sum += state[5] * cumulative_wells(board);
    return sum;
}

int movement(int state[], int buffer[], int len, int i)
{
    if (i >= len) {
        return 0;
    }

    for (int j = 0; j < len; j++) {
        buffer[j] = state[j];
    }

    buffer[i] = buffer[i] + rand() % 10 - 5;

    return 1;
}

int generate_board(struct board * board, int state[], int len)
{
    // this program is just pseudo random, since the seed is actually the same everytime.
    srand(2);

    int lines_filled = 0;

    #ifdef PERFORMANCE_METER
    int msec = 0;
    clock_t before = clock();
    #endif

    struct piece * piece;
    for (int p = 0; p < input->n_pieces; p++) {
        #ifdef PERFORMANCE_METER
        if (p % 25 == 0) {
            msec = 0;
            before = clock();
        }
        #endif

        int type = input->pieces[p];

        piece = piece_new(type);

        int first = 1;
        int best_fitness = 0;
        int best_position = 0;
        int best_rotation = 0;

        for (int r = 0; r < piece_rotation_count(piece); r++) {
            piece_rotate(piece);

            for (int x = 0; x < board->width - piece_width(piece) + 1; x++) {
                int y = board_collission_height(board, piece, x);

                // ignore solution if the piece will overflow the board.
                if (y > board->height - piece_height(piece)) {
                    continue;
                }

                board_place_piece(board, piece, x, y, 1);

                // calculate fitness of the board.
                int fitness = evaluate_board(board, state, len);

                if (fitness >= best_fitness || first == 1) {
                    first = 0;
                    best_fitness = fitness;
                    best_position = x;
                    best_rotation = r;
                }

                board_purge_ghosts(board);
            }
        }

        piece_free(&piece);

        if (first == 0) {
            // get best piece and place it in the board, embracing it for the
            // new state.
            piece = piece_new_with_rotation(type, best_rotation);
            int y = board_collission_height(board, piece, best_position);
            board_place_piece(board, piece, best_position, y, 0);

            lines_filled += board_embrace_ghosts(board);

            piece_free(&piece);
        } else {
            // in this case we didn't found any place to put the piece
            // without overflowing the board, terminate.
            break;
        }

        #ifdef PERFORMANCE_METER
        if (p % 25 == 24) {
            clock_t difference = clock() - before;
            msec = difference * 1000 / CLOCKS_PER_SEC;
            printf("  -> %f mspp (prove of 25)\n", (float) msec / 25);
        }
        #endif
    }

    return lines_filled;
}

int evaluate(int state[], int len)
{
    struct board * board = board_new(input->width, input->height);
    
    int lines_filled = generate_board(board, state, len);

    #ifdef KEEP_LINES
    lines_filled = 0;
    for (int y = 0; y < board->height; y++) {
        lines_filled += board_is_row_filled(board, y);
    }
    #endif

    board_free(&board);

    return lines_filled;
}

int stop(int iterations)
{
    return iterations >= 10;
}

int main(int argc, char * argv[])
{
    input = input_read(argv[1]);

    // tabu search usage.
    int initial[6] = { 0, 0, 0, 0, 0, 0 };
    int solution[6];

    // a tabu search framework was developed for this program in order to
    // improve modularity of the program. More information can be found
    // in the tabu.h header file.
    struct tabu * tabu = tabu_new(evaluate, movement, stop);

    clock_t before = clock();
    int best_fitness = tabu_execute(tabu, initial, solution, 6, 5);
    clock_t difference = clock() - before;
    int msec = difference * 1000 / CLOCKS_PER_SEC;

    printf("\n\nresults:\n");
    printf("  full lines: %d\n", best_fitness);
    printf("  weights:    %d, %d, %d, %d, %d, %d\n", solution[0], solution[1], 
            solution[2], solution[3], solution[4], solution[5]);
    printf("  time:       %dms\n", msec);

    printf("  generated board:\n");
    struct board * board = board_new(input->width, input->height);
    generate_board(board, solution, 6);
    board_dump(board);

    #ifdef LATEX_OUTPUT
    printf("%d & %d & %d & %d, %d, %d, %d, %d, %d & %d & %d \\\\\n", input->width, 
            input->height, input->n_pieces, solution[0], solution[1], 
            solution[2], solution[3], solution[4], solution[5], best_fitness, msec);
    #endif

    board_free(&board);
    tabu_free(&tabu);
    input_free(&input);

    return 0;
}
