#include "input.h"

#include <stdio.h>
#include <stdlib.h>

#define DEBUG

void _read_dimensions(FILE * file, int * width, int * height, int * n_pieces)
{
    // read board dimensions.
    fscanf(file, "%d %d", width, height);

    // read piece count.
    fscanf(file, "%d", n_pieces);

    #ifdef DEBUG
    printf("  dimensions: %d x %d\n", *width, *height);
    printf("  pieces: %d\n", *n_pieces);
    #endif
}

void _read_pieces_definitions(FILE * file)
{
    char * line = NULL;
    size_t length = 0;

    // ignore the first line.
    getline(&line, &length, file);

    // this line will contain the actual definitions.
    getline(&line, &length, file);

    #ifdef DEBUG
    printf("  piece definitions: %s", line);
    #endif

    free(line);
}

void _read_pieces(FILE * file, struct input * self)
{
    for (int i = 0; i < self->n_pieces; i++) {
        fscanf(file, "%d", &self->pieces[i]);
    }

    #ifdef DEBUG
    printf("  pieces: ");
    for (int i = 0; i < self->n_pieces && i < 20; i++) {
        printf("%d ", self->pieces[i]);
    }
    if (self->n_pieces >= 20) {
        printf("...");
    }
    printf("\n");
    #endif
}

struct input * input_read(char * path)
{
    FILE * file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "input file does not exists\n");
        exit(0);
    }

    #ifdef DEBUG
    printf("reading instance %s...\n", path);
    #endif

    // read structure header.
    int width, height, n_pieces;

    _read_dimensions(file, &width, &height, &n_pieces);
    _read_pieces_definitions(file);

    // construct structure.
    struct input * self = malloc(sizeof(struct input) + n_pieces * sizeof(int));
    self->width = width;
    self->height = height;
    self->n_pieces = n_pieces;

    // read all pieces and put them in the structure.
    _read_pieces(file, self);

    // we are done!
    fclose(file);

    #ifdef DEBUG
    printf("done reading instance %s.\n", path);
    #endif

    return self;
}

void input_free(struct input ** self)
{
    free(*self);
    *self = NULL;
}
