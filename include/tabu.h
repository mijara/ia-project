/**
 * A simple Tabu Search framework.
 * 
 * This framework helps in finding solutions using the Tabu Search algorithm,
 * where a movement, evaluation and stop condition functions are required.
 * 
 * - The stop function receives an integer with the currentnumber of 
 *   iterations. Should return 1 if the program needs to stop. For example:
 *     >> return iterations >= 20 - 1;
 *   is a condition for 20 iterations (see tabu_stop).
 * 
 * - The movemement function should update the state in order to generate
 *   all neightbours. An integer will be passed so that the function knows
 *   which neightbour to generate. Should return 1 if there's more neightbours
 *   to generate and 0 if this was the last one (see tabu_mov).
 * 
 * - The evaluation functions receives a state and should return the fitness
 *   of the evaluation (see tabu_eval).
 * 
 * To execute the framework see tabu_execute().
 * 
 * Author: Marcelo Jara <marjara35@gmail.com>
 */
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

/**
 * Creates a new instance of the tabu algorithm.
 */
struct tabu * tabu_new(tabu_eval evaluate, tabu_mov movement, tabu_stop stop);

/**
 * Frees an instance of this class created with tabu_new().
 */
void tabu_free(struct tabu ** self);

/**
 * Executes the tabu algorithm. The programmer will need the tabu instance,
 * the initial solution, a buffer (which will be filled with the final 
 * solution), the solution length and a tabu list size.
 * 
 * This functions returns the solution via the buffer param and returns
 * the best fitness found.
 */
int tabu_execute(struct tabu * self, int initial[], int buffer[], 
        int state_len, int tabu_size);

#endif // TABU_H
