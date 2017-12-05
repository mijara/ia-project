#ifndef BOARD_H
#define BOARD_H

#include "piece.h"

#define BOARD_BLOCK_EMPTY 0
#define BOARD_BLOCK_OCCUPIED 1
#define BOARD_BLOCK_GHOST 2

struct board 
{
    int width;
    int height;
    int blocks[];
};

/**
 * Board constructor, takes a width and a height, return a board.
 * Free with board_free.
 */
struct board * board_new(int width, int height);

/**
 * Deconstructs a board. Sets the pointer to NULL.
 */
void board_free(struct board ** self);

/**
 * Sets a board cell to some value. One of:
 * BOARD_BLOCK_EMPTY, BOARD_BLOCK_OCCUPIED, BOARD_BLOCK_GHOST
 */
void board_set(struct board * self, int x, int y, int val);

/**
 * Gets value from a board cell. One of:
 * BOARD_BLOCK_EMPTY, BOARD_BLOCK_OCCUPIED, BOARD_BLOCK_GHOST
 */
int board_get(struct board * self, int x, int y);

/**
 * Returns 1 if a board cell is occupied or ghost, false if it is empty.
 */
int board_is_occupied(struct board * self, int x, int y);

/**
 * Prints the board to stdout with a pretty format.
 */
void board_dump(struct board * self);

/**
 * Purges all board ghost cells, setting them to BOARD_BLOCK_EMPTY.
 */
void board_purge_ghosts(struct board * self);

/**
 * Materializes all ghosts, turning them into occupied cells.
 */
int board_embrace_ghosts(struct board * self);

/**
 * Returns 1 if the row is filled with occupied and ghosts board cells.
 */
int board_is_row_filled(struct board * self, int y);

/**
 * Returns the height of the column.
 */
int board_column_height(struct board * self, int x);

/**
 * Returns the height at which the piece will first collide with the
 * board. The piece shall be placed at this height.
 */
int board_collission_height(struct board * self, struct piece * piece, int x);

void board_place_piece(struct board * self, struct piece * piece, int x, int y);

#endif // BOARD_H
