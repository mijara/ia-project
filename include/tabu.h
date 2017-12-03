#ifndef TABU_H
#define TABU_H

struct callbacks {
    int (*should_stop)(int iterations);
    int (*movement)(int state[], int buffer[], int len, int i);
    int (*evaluate)(int state[], int len);
};

void tabu(int initial[], int buffer[], int len, struct callbacks cbs);

#endif // TABU_H
