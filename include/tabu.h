#ifndef TABU_H
#define TABU_H

struct tabu_list {
    int ** list;
    int index;
    int capacity;
};

struct callbacks {
    int (*should_stop)(int iterations);
    int (*movement)(int state[], int buffer[], int len, int i);
    int (*evaluate)(int state[], int len);
};

struct tabu_list * tabu_list_new();

void tabu(int initial[], int buffer[], int len, struct callbacks cbs, int tabu_size);

#endif // TABU_H
