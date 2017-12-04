#include "board.h"
#include "stdlib.h"
#include "stdio.h"

struct board * board_new(int width, int height)
{
    struct board * self = malloc(sizeof(struct board) + sizeof(int) * width * height);

    self->width = width;
    self->height = height;

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
            } else {
                printf("%d", board_get(self, x, y));
            }
        }
        printf("|\n");
    }
}

void board_purge_ghosts(struct board * self)
{
    for (int x = 0; x < self->width; x++) {
        for (int y = 0; y < self->height; y++) {
            board_set(self, x, y, BOARD_BLOCK_EMPTY);
        }
    }
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
    int max_height = 0;

    for (int y = 0; y < self->height; y++) {
        if (board_is_occupied(self, x, y)) {
            max_height = y;
        }
    }

    return max_height + 1;
}
