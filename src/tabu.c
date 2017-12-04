#include "tabu.h"
#include "stdio.h"
#include "string.h"
#include "tabu_list.h"

int _best_neighbour(int state[], int best_neighbour[], int len, struct callbacks cbs,
        struct tabu_list * tabu_list) {
    int i = 0;
    int best_fitness = 0;

    int buffer[len];
    while (cbs.movement(state, buffer, len, i++)) {
        int fitness = cbs.evaluate(buffer, len);
        printf("fitness: %d\n", fitness);

        if (tabu_list_contains(tabu_list, buffer)) {
            printf("  ignored\n");
            continue;
        }

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
    int state_len,
    struct callbacks cbs,
    int tabu_size
) {
    struct tabu_list * tabu_list = tabu_list_new(tabu_size, state_len);

    int best_solution[state_len];
    int best_fitness = cbs.evaluate(state, state_len);

    int i = 0;
    while (!cbs.should_stop(i++)) {
        // find best neightbour and set as the new state.
        int best_neighbour[state_len];
        int fitness = _best_neighbour(state, best_neighbour, state_len, cbs, tabu_list);
        memcpy(state, best_neighbour, state_len * sizeof(int));

        // insert to tabu list.
        tabu_list_insert(tabu_list, state);

        // if the best neightbour has a better fitness than the best fitness till now, replace.
        if (fitness > best_fitness) {
            best_fitness = fitness;
            memcpy(best_solution, best_neighbour, state_len * sizeof(int));
        }
    }

    memcpy(buffer, best_solution, state_len * sizeof(int));
}
