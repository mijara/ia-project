#include "piece.h"
#include "stdlib.h"
#include "stdio.h"

#define ROTATE(__PIECE__, __ROTATIONS__) \
        self->rotation = (self->rotation + 1) % __ROTATIONS__; \
        self->data = __PIECE__[self->rotation];

int piece_z[2][8] = {
    { 0, 1, 1, 1, 1, 0, 2, 0 },
    { 0, 0, 0, 1, 1, 1, 1, 2 }, 
};

int piece_s[2][8] = {
    { 0, 0, 1, 0, 1, 1, 2, 1 },
    { 0, 1, 0, 2, 1, 0, 1, 1 }
};

int piece_t[4][8] = {
    { 0, 0, 1, 0, 2, 0, 1, 1 },
    { 0, 0, 0, 1, 0, 2, 1, 1 },
    { 0, 1, 1, 1, 2, 1, 1, 0 },
    { 0, 1, 1, 0, 1, 1, 1, 2 }
};

int piece_o[1][8] = {
    { 0, 0, 1, 1, 0, 1, 1, 0 }
};

int piece_j[4][8] = {
    { 0, 0, 1, 0, 1, 1, 1, 2 },
    { 0, 0, 1, 0, 2, 0, 0, 1 },
    { 0, 0, 0, 1, 0, 2, 1, 2 },
    { 0, 1, 1, 1, 2, 1, 2, 0 }
};

int piece_l[4][8] = {
    { 0, 0, 0, 1, 0, 2, 1, 0 },
    { 0, 0, 0, 1, 1, 1, 2, 1 },
    { 1, 0, 1, 1, 1, 2, 0, 2 },
    { 0, 0, 1, 0, 2, 0, 2, 1 }
};

int piece_i[2][8] = {
    { 0, 0, 0, 1, 0, 2, 0, 3 },
    { 0, 0, 1, 0, 2, 0, 3, 0 }
};

int widths[7][4] = {
    { 3, 2, 0, 0 },
    { 3, 2, 0, 0 },
    { 3, 2, 3, 2 },
    { 2, 0, 0, 0 },
    { 2, 3, 2, 3 },
    { 2, 3, 2, 3 },
    { 1, 4, 0, 0 },
};

int heights[7][4] = {
    { 2, 3, 0, 0 },
    { 2, 3, 0, 0 },
    { 2, 3, 2, 3 },
    { 2, 0, 0, 0 },
    { 3, 2, 3, 2 },
    { 3, 2, 3, 2 },
    { 4, 1, 0, 0 },
};

int rotations[7] = {
    2, 2, 4, 1, 4, 4, 2
};

struct piece * piece_new(int type)
{
    int * data = NULL;

    switch (type) {
    case 0:
        data = piece_z[0];
        break;
    case 1:
        data = piece_s[0];
        break;
    case 2:
        data = piece_t[0];
        break;
    case 3:
        data = piece_o[0];
        break;
    case 4:
        data = piece_j[0];
        break;
    case 5:
        data = piece_l[0];
        break;
    case 6:
        data = piece_i[0];
        break;
    }

    struct piece * self = malloc(sizeof(struct piece));
    self->data = data;
    self->type = type;
    self->rotation = -1;
    return self;
}

struct piece * piece_new_with_rotation(int type, int rotation)
{
    struct piece * self = piece_new(type);

    for (int i = 0; i < rotation + 1; i++) {
        piece_rotate(self);
    }

    return self;
}

void piece_free(struct piece ** self)
{
    free(*self);
    *self = NULL;
}

void piece_dump(struct piece * self)
{
    int matrix[4][4] = { 0 };
    matrix[self->data[0]][self->data[1]] = 1;
    matrix[self->data[2]][self->data[3]] = 1;
    matrix[self->data[4]][self->data[5]] = 1;
    matrix[self->data[6]][self->data[7]] = 1;

    for (int j = 3; j >= 0; j--) {
        for (int i = 0; i < 4; i++) {
            printf("%d", matrix[i][j]);
        }

        printf("\n");
    }
}

void piece_rotate(struct piece * self)
{
    switch (self->type) {
    case 0:
        ROTATE(piece_z, rotations[self->type])
        break;
    case 1:
        ROTATE(piece_s, rotations[self->type])
        break;
    case 2:
        ROTATE(piece_t, rotations[self->type])
        break;
    case 3:
        ROTATE(piece_o, rotations[self->type])
        break;
    case 4:
        ROTATE(piece_j, rotations[self->type])
        break;
    case 5:
        ROTATE(piece_l, rotations[self->type])
        break;
    case 6:
        ROTATE(piece_i, rotations[self->type])
        break;
    }
}

int piece_column_lowest(struct piece * self, int x)
{
    int lowest = 99;

    for (int i = 0; i < 4; i++) {
        if (x == self->data[i * 2]) {
            int y = self->data[i * 2 + 1];
            if (y < lowest) {
                lowest = y;
            }
        }
    }

    if (lowest == 99) {
        return -1;
    }

    return lowest;
}

int piece_width(struct piece * self)
{
    return widths[self->type][self->rotation];
}

int piece_height(struct piece * self)
{
    return heights[self->type][self->rotation];
}

int piece_rotation_count(struct piece * self)
{
    return rotations[self->type];
}
