#include "tabu_list.h"
#include "stdlib.h"
#include "string.h"

int _state_equals(int list[], int state[], int len)
{
    for (int i = 0; i < len; i++) {
        if (list[i] != state[i]) {
            return 0;
        }
    }

    return 1;
}

struct tabu_list * tabu_list_new(int capacity, int state_length)
{
    // malloc the size of the struct plus the extra size of the state array.
    int byte_size = sizeof(struct tabu_list) + capacity * state_length * sizeof(int);
    struct tabu_list * self = malloc(byte_size);

    // initialize the state of the tabu_list.
    self->capacity = capacity;
    self->index = 0;
    self->state_length = state_length;

    return self;
}

void tabu_list_free(struct tabu_list ** self)
{
    free(*self);
    *self = NULL;
}

void tabu_list_insert(struct tabu_list * self, int state[])
{
    int byte_size = self->state_length * sizeof(int);
    memcpy(&self->list[self->index * self->state_length], state, byte_size);
    self->index = (self->index + 1) % self->capacity;
}

int tabu_list_contains(struct tabu_list * self, int state[])
{
    for (int i = 0; i < self->capacity; i++) {
        if (_state_equals(&self->list[i * self->state_length], state, self->state_length)) {
            return 1;
        }
    }

    return 0;
}
