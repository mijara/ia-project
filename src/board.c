#include "board.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

struct board * board_new(int width, int height)
{
    static int id = 0;

    struct board * self = malloc(
        sizeof(struct board) + sizeof(int) * width * height);

    self->width = width;
    self->height = height;
    self->ghost_min_x_prev = 0;
    self->ghost_max_x_prev = 0;
    self->ghost_min_x = 0;
    self->ghost_max_x = 0;
    self->uid = id++;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            board_set(self, x, y, BOARD_BLOCK_EMPTY);
        }
    }

    return self;
}

void board_free(struct board ** self)
{
    free(*self);
    *self = NULL;
}

inline void board_set(struct board * self, int x, int y, int val)
{
    if (y >= self->height) {
        board_dump(self);
        exit(1);
    }

    self->blocks[y * self->width + x] = val;
}

inline int board_get(struct board * self, int x, int y)
{
    return self->blocks[y * self->width + x];
}

inline int board_is_occupied(struct board * self, int x, int y)
{
    int cell = self->blocks[y * self->width + x];
    if (cell == BOARD_BLOCK_GHOST || cell == BOARD_BLOCK_OCCUPIED) {
        return BOARD_BLOCK_OCCUPIED;
    }
    return BOARD_BLOCK_EMPTY;
}

void board_dump(struct board * self)
{
    for (int y = self->height - 1; y >= 0; y--) {
        printf("|");
        for (int x = 0; x < self->width; x++) {
            int v = board_get(self, x, y);
            if (v == 0) {
                printf(" ");
            } else if (v == BOARD_BLOCK_OCCUPIED) {
                printf("#");
            } else if (v == BOARD_BLOCK_GHOST) {
                printf("@");
            }
        }
        printf("|\n");
    }
}

void board_purge_ghosts(struct board * self)
{
    for (int i = 0; i < 4; i++) {
        int x = self->ghosts[i * 2];
        int y = self->ghosts[i * 2 + 1];

        board_set(self, x, y, BOARD_BLOCK_EMPTY);
    }
}

int board_embrace_ghosts(struct board * self)
{
    for (int x = 0; x < self->width; x++) {
        for (int y = 0; y < self->height; y++) {
            if (board_get(self, x, y) == BOARD_BLOCK_GHOST) {
                board_set(self, x, y, BOARD_BLOCK_OCCUPIED);
            }
        }
    }

    int filled = 0;

    #ifndef KEEP_LINES
    // remove filled lines.
    for (int y = self->height - 1; y >= 0; y--) {
        if (board_is_row_filled(self, y)) {
            filled++;
            // move rows above to one below.
            for (int j = y + 1; j < self->height; j++) {
                for (int x = 0; x < self->width; x++) {
                    board_set(self, x, j - 1, board_get(self, x, j));
                }
            }

            for (int x = 0; x < self->width; x++) {
                board_set(self, x, self->height - 1, 0);
            }
        }
    }
    #else
    filled = -1;
    #endif

    return filled;
}

int board_is_row_filled(struct board * self, int y)
{
    int c = self->width;

    for (int x = 0; x < self->width; x++) {
        c -= board_get(self, x, y) >= BOARD_BLOCK_OCCUPIED;
    }

    return !c;
}

int board_column_height(struct board * self, int x)
{
    for (int y = self->height - 1; y >= 0; y--) {
        if (board_is_occupied(self, x, y)) {
            return y + 1;
        }
    }

    return 0;
}

int _minv(int vals[], int len)
{
    int min = 99;

    for (int i = 0; i < len; i++) {
        if (vals[i] < min) {
            min = vals[i];
        }
    }

    return min;
}

int board_collission_height(struct board * self, struct piece * piece, int x)
{
    int heights[4] = {
        board_column_height(self, x + 0),
        board_column_height(self, x + 1),
        board_column_height(self, x + 2),
        board_column_height(self, x + 3),
    };

    int diffs[4] = { 0 };
    int width = piece_width(piece);

    for (int i = 0; i < width; i++) {
        diffs[i] = piece_column_lowest(piece, i) + self->height - heights[i];
    }

    int min = _minv(diffs, width);
    return self->height - min;
}

void board_place_piece(struct board * self, struct piece * piece, int x, int y, int update_ghosts)
{
    if (update_ghosts) {
        self->ghost_min_x_prev = self->ghost_min_x;
        self->ghost_max_x_prev = self->ghost_max_x;

        self->ghost_min_x = x - (x > 0 ? 1 : 0);
        self->ghost_max_x = x + piece_width(piece);
    }

    for (int i = 0; i < 4; i++) {
        int piece_x = self->ghosts[i * 2] = piece->data[i * 2] + x;
        int piece_y = self->ghosts[i * 2 + 1] = piece->data[i * 2 + 1] + y;

        board_set(self, piece_x, piece_y, BOARD_BLOCK_GHOST);
    }
}
