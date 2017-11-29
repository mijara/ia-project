#ifndef BOARD_H
#define BOARD_H

#define BOARD_BLOCK_EMPTY 0
#define BOARD_BLOCK_OCCUPIED 1

struct board {
    int width;
    int height;
    int blocks[];
};

struct board * board_new(int width, int height);
void board_free(struct board ** self);
void board_set(struct board * self, int x, int y, int val);
int board_get(struct board * self, int x, int y);
void board_dump(struct board * self);

#endif // BOARD_H