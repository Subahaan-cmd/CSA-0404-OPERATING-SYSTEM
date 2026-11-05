/* Q6 D/E: time of the safety algorithm for n = 10, 100, 1000 processes and m = 3 resource types.
   Worst case: only one process can finish in each pass, so n passes of n checks each = O(m n^2). */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define M 3
static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec / 1e9; }
static long checks;
static int is_safe(int n, int (*maxm)[M], int (*alloc)[M], int *avail)
{
    int work[M], *finish = calloc(n, sizeof(int)), count = 0; memcpy(work, avail, sizeof work);
    while (count < n) {
        int found = 0;
        for (int p = 0; p < n; p++) {
            if (finish[p]) continue;
            int ok = 1; checks++;
            for (int r = 0; r < M; r++) if (maxm[p][r] - alloc[p][r] > work[r]) { ok = 0; break; }
            if (ok) { for (int r = 0; r < M; r++) work[r] += alloc[p][r]; finish[p] = 1; count++; found = 1; }
        }
        if (!found) { free(finish); return 0; }
    }
    free(finish); return 1;
}
int main(void)
{
    int sizes[] = {10, 100, 1000, 4000};
    printf("%6s %14s %14s %12s %14s\n", "n", "checks (worst)", "time worst(us)", "checks (rand)", "time rand(us)");
    for (int k = 0; k < 4; k++) {
        int n = sizes[k];
        int (*maxm)[M] = malloc(n * sizeof *maxm), (*alloc)[M] = malloc(n * sizeof *alloc), avail[M] = {0, 0, 0};
        /* worst case: process p needs (n-1-p) units of resource 0 and holds 1; it can finish only after the later ones */
        for (int p = 0; p < n; p++) { alloc[p][0] = 1; alloc[p][1] = 0; alloc[p][2] = 0; maxm[p][0] = 1 + (n - 1 - p); maxm[p][1] = 0; maxm[p][2] = 0; }
        checks = 0; double t0 = now(); int reps = n <= 100 ? 2000 : 1; for (int i = 0; i < reps; i++) is_safe(n, maxm, alloc, avail);
        double tw = (now() - t0) / reps * 1e6; long cw = checks / reps;
        srand(5);
        for (int p = 0; p < n; p++) for (int r = 0; r < M; r++) { alloc[p][r] = rand() % 3; maxm[p][r] = alloc[p][r] + rand() % 4; }
        int av2[M] = {6, 6, 6}; checks = 0; t0 = now(); for (int i = 0; i < reps; i++) is_safe(n, maxm, alloc, av2);
        double tr = (now() - t0) / reps * 1e6; long cr = checks / reps;
        printf("%6d %14ld %14.1f %12ld %14.1f\n", n, cw, tw, cr, tr);
        free(maxm); free(alloc);
    }
    return 0;
}
