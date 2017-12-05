#ifndef PIECE_H
#define PIECE_H

/**
 * The intention of this representation is to reduce the memory and processing
 * footprint by making each piece a statically defined structure.
 */
struct piece
{
    int * data;
    int type;
    int rotation;
};

struct piece * piece_new(int type);

struct piece * piece_new_with_rotation(int type, int rotation);

void piece_free(struct piece ** self);

void piece_dump(struct piece * self);

void piece_rotate(struct piece * self);

/**
 * Returns the piece's column lowest cell.
 * Returns -1 when there's no pieces in that column.
 */
int piece_column_lowest(struct piece * self, int x);

int piece_width(struct piece * self);

int piece_height(struct piece * self);

int piece_rotation_count(struct piece * self);

#endif // PIECE_H
