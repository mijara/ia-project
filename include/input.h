#ifndef INPUT_H
#define INPUT_H

struct input {
    int width;
    int height;
    int n_pieces;
    int pieces[];
};

struct input * input_read(char * path);

void input_free(struct input ** self);

#endif // INPUT_H
