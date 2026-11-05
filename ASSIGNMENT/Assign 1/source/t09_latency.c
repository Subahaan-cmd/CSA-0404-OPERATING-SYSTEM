/* Task 9: measure access time per element for working sets from 4 KB to 256 MB (pointer chasing). */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec / 1e9; }
int main(void)
{
    printf("%10s %12s\n", "size", "ns/access");
    for (size_t kb = 4; kb <= 262144; kb *= 4) {
        size_t n = kb * 1024 / sizeof(void *);
        void **a = malloc(n * sizeof(void *));
        size_t *perm = malloc(n * sizeof(size_t));
        for (size_t i = 0; i < n; i++) perm[i] = i;
        srand(1);
        for (size_t i = n - 1; i > 0; i--) { size_t j = (size_t)rand() % (i + 1); size_t t = perm[i]; perm[i] = perm[j]; perm[j] = t; }
        for (size_t i = 0; i < n; i++) a[perm[i]] = &a[perm[(i + 1) % n]];
        void **p = &a[perm[0]];
        long steps = 20000000;
        double t0 = now();
        for (long s = 0; s < steps; s++) p = (void **)*p;
        double t1 = now();
        if (!p) return 1;
        if (kb >= 1024) printf("%7zu MB %12.2f\n", kb / 1024, (t1 - t0) * 1e9 / steps);
        else printf("%7zu KB %12.2f\n", kb, (t1 - t0) * 1e9 / steps);
        free(a); free(perm);
    }
    return 0;
}
