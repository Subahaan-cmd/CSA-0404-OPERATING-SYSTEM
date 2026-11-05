#include <stdio.h>
#define N 4
#ifndef QUANTUM
#define QUANTUM 3
#endif
int main(void)
{
    const char *name[N] = {"P1", "P2", "P3", "P4"};
    int arrival[N] = {0, 1, 2, 3};
    int burst[N] = {8, 4, 9, 5};
    int quantum = QUANTUM;
    int left[N], finish[N], queue[64], head = 0, tail = 0, added[N] = {0};
    for (int i = 0; i < N; i++) left[i] = burst[i];
    int time = 0, done = 0, switches = 0, last = -1;
    queue[tail++] = 0;
    added[0] = 1;
    printf("Gantt:");
    while (done < N) {
        int p = queue[head++];
        /* TODO 1 (completed) */
        int run = left[p] < quantum ? left[p] : quantum;   /* min(quantum, left[p])     */
        printf(" %s(%d-%d)", name[p], time, time + run);   /* print the Gantt segment   */
        time += run;                                       /* advance the clock         */
        left[p] -= run;                                    /* reduce remaining time     */
        for (int i = 0; i < N; i++)                        /* admit new arrivals first  */
            if (!added[i] && arrival[i] <= time) { queue[tail++] = i; added[i] = 1; }
        if (left[p] > 0) queue[tail++] = p;                /* back to the end of queue  */
        else { finish[p] = time; done++; }                 /* finished: record the time */
        if (last >= 0 && last != p) switches++;            /* CPU changed process       */
        last = p;
    }
    double tw = 0, tt = 0;
    printf("\nProcess TAT WT\n");
    for (int i = 0; i < N; i++) {
        int tat = finish[i] - arrival[i], wt = tat - burst[i];
        tt += tat;
        tw += wt;
        printf("%-7s %4d %3d\n", name[i], tat, wt);
    }
    printf("Average TAT = %.2f, average WT = %.2f\n", tt / N, tw / N);
    printf("(context switches = %d)\n", switches);
    return 0;
}
