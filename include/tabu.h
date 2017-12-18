#ifndef TABU_H
#define TABU_H

typedef int (*tabu_stop)(int iters);

typedef int (*tabu_mov)(int state[], int buffer[], int len, int i);

typedef int (*tabu_eval)(int state[], int len);

struct tabu
{
    tabu_eval evaluate;
    tabu_mov movement;
    tabu_stop stop;
};

struct tabu * tabu_new(tabu_eval evaluate, tabu_mov movement, tabu_stop stop);

void tabu_free(struct tabu ** self);

int tabu_execute(struct tabu * self, int initial[], int buffer[], int state_len, int tabu_size);

#endif // TABU_H
