#include "tabu.h"
#include "stdio.h"
#include "string.h"

int _best_neighbour(int state[], int best_neighbour[], int len, struct callbacks cbs) {
    int i = 0;
    int best_fitness = 0;

    int buffer[len];
    while (cbs.movement(state, buffer, len, i++)) {
        int fitness = cbs.evaluate(buffer, len);
        printf("evaluate: %d\n", fitness);

        if (fitness > best_fitness) {
            best_fitness = fitness;
            printf("  new best.\n");

            memcpy(best_neighbour, buffer, len * sizeof(int));
        }
    }

    return best_fitness;
}

void tabu(
    int state[],
    int buffer[],
    int len,
    struct callbacks cbs
) {
    int best_solution[len];
    int best_fitness = cbs.evaluate(state, len);

    int i = 0;
    while (!cbs.should_stop(i++)) {
        // find best neightbour and set as the new state.
        int best_neighbour[len];
        int fitness = _best_neighbour(state, best_neighbour, len, cbs);
        memcpy(state, best_neighbour, len * sizeof(int));

        // if the best neightbour has a better fitness than the best fitness till now, replace.
        if (fitness > best_fitness) {
            best_fitness = fitness;
            memcpy(best_solution, best_neighbour, len * sizeof(int));
        }
    }

    memcpy(buffer, best_solution, len * sizeof(int));
}
