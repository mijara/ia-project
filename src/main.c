#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "board.h"
#include "tabu.h"
#include "features.h"
#include "piece.h"
#include "input.h"

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

int evaluate(int state[], int len)
{
    struct board * board = board_new(input->width, input->height);
    int lines_filled = 0;

    struct piece * piece;
    for (int p = 0; p < input->n_pieces; p++) {
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

                board_place_piece(board, piece, x, y);

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
            board_place_piece(board, piece, best_position, y);
            lines_filled += board_embrace_ghosts(board);
            piece_free(&piece);
        } else {
            // in this case we didn't found any place to put the piece
            // without overflowing the board, terminate.
            break;
        }
    }

    board_free(&board);

    return lines_filled;
}

int stop(int iterations)
{
    printf("iteration %d.\n", iterations + 1);
    return iterations >= 20 - 1;
}

int main(int argc, char * argv[])
{
    // this program is just pseudo random, since the seed is actually the same everytime.
    srand(1);

    input = input_read(argv[1]);

    // tabu search usage.
    int initial[6] = { 0, 0, 0, 0, 0, 0 };
    int solution[6];

    struct tabu * tabu = tabu_new(evaluate, movement, stop);

    int best_fitness = execute(tabu, initial, solution, 6, 5);

    printf("lines cleared: %d\n", best_fitness);
    printf("weights: %d, %d, %d, %d, %d, %d\n", solution[0], solution[1], solution[2], solution[3], solution[4], solution[5]);

    tabu_free(&tabu);
    input_free(&input);

    return 0;
}
