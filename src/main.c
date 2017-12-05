#include "stdio.h"
#include "board.h"
#include "tabu.h"
#include "string.h"
#include "features.h"
#include "piece.h"

#include <time.h>
#include <stdlib.h>

int evaluate_board(struct board * board, int state[]) {
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
    srand(time(NULL));

    int pieces_count = 8;
    int pieces[] = {
        6, 3, 5, 6, 2, 3, 1, 2
    };

    int weights[] = { -1, 1, -1, -1, -4, -1 };

    /*
    // tabu search usage.
    int initial[5] = { 0, 0, 0, 0, 0 };
    int solution[5];

    struct tabu * tabu = tabu_new(evaluate, movement, stop);

    execute(tabu, initial, solution, 5, 3);

    printf("found best: %d\n", evaluate(solution, 5));
    */

    struct board * board = board_new(10, 20);
    int filled = 0;

    struct piece * piece;
    for (int p = 0; p < 1000; p++) {
        int type = rand() % 7;

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
                int fitness = evaluate_board(board, weights);

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
            filled += board_embrace_ghosts(board);
            piece_free(&piece);
        } else {
            // in this case we didn't found any place to put the piece
            // without overflowing the board, terminate.
            break;
        }
    }

    board_dump(board);
    printf("%d\n", filled);

    board_free(&board);

    return 0;
}
