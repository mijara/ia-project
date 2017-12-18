#include "tabu.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "tabu_list.h"
#include "limits.h"

struct tabu * tabu_new(tabu_eval evaluate, tabu_mov movement, tabu_stop stop)
{
    struct tabu * self = malloc(sizeof(struct tabu)); 
    
    self->evaluate = evaluate;
    self->movement = movement;
    self->stop = stop;
    
    return self;
}

void tabu_free(struct tabu ** self)
{
    free(*self);
    *self = NULL;
}

int _best_neighbour(struct tabu * self, int state[], int best_neighbour[], int len,
        struct tabu_list * tabu_list)
{
    int i = 0;
    int best_fitness = INT_MIN;

    int buffer[len];

    while (self->movement(state, buffer, len, i++)) {
        int fitness = self->evaluate(buffer, len);
        // printf("fitness: %d\n", fitness);

        if (tabu_list_contains(tabu_list, buffer)) {
            // printf("  ignored\n");
            continue;
        }

        if (fitness >= best_fitness) {
            // printf("  new best\n");
            best_fitness = fitness;
            memcpy(best_neighbour, buffer, len * sizeof(int));
        }
    }

    return best_fitness;
}

int tabu_execute(struct tabu * self, int state[], int buffer[], int state_len, 
        int tabu_size)
{
    struct tabu_list * tabu_list = tabu_list_new(tabu_size, state_len);

    int best_solution[state_len];
    memcpy(best_solution, state, state_len * sizeof(int));
    int best_fitness = self->evaluate(state, state_len);

    int i = 0;
    while (!self->stop(i++)) {
        // find best neightbour and set as the new state.
        int best_neighbour[state_len];

        int fitness = _best_neighbour(self, state, best_neighbour, state_len, 
                tabu_list);
        memcpy(state, best_neighbour, state_len * sizeof(int));

        // insert to tabu list.
        tabu_list_insert(tabu_list, state);

        // if the neightbour has a better fitness than the best fitness till 
        // now, replace.
        if (fitness >= best_fitness) {
            best_fitness = fitness;
            memcpy(best_solution, best_neighbour, state_len * sizeof(int));
        }

        printf("iteration %d:\n", i);
        printf("  weights: %d, %d, %d, %d, %d, %d\n", best_neighbour[0], best_neighbour[1], best_neighbour[2], best_neighbour[3], best_neighbour[4], best_neighbour[5]);
        printf("  lines cleared: %d\n", best_fitness);
    }

    memcpy(buffer, best_solution, state_len * sizeof(int));

    tabu_list_free(&tabu_list);

    return best_fitness;
}
