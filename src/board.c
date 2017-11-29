#include "board.h"
#include "stdlib.h"
#include "stdio.h"

struct board * board_new(int width, int height) {
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

void board_free(struct board ** self) {
    free(*self);
    *self = NULL;
}

inline void board_set(struct board * self, int x, int y, int val) {
    self->blocks[y * self->width + x] = val;
}

inline int board_get(struct board * self, int x, int y) {
    return self->blocks[y * self->width + x];
}

void board_dump(struct board * self) {
    for (int y = self->height - 1; y >= 0; y--) {
        for (int x = 0; x < self->width; x++) {
            printf("%d", board_get(self, x, y));
        }
        printf("\n");
    }
}
