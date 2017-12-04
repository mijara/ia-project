#ifndef TABU_LIST_H
#define TABU_LIST_H

struct tabu_list {
    int index;
    int capacity;
    int state_length;
    int list[];
};

struct tabu_list * tabu_list_new(int capacity, int state_length);
void tabu_list_insert(struct tabu_list * self, int state[]);

#endif // TABU_LIST_H
