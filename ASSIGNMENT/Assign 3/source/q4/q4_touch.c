/* Touch 200 MB of memory in sequential and in random order, count page faults and time the second pass. */
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>
static long minflt(void) { struct rusage r; getrusage(RUSAGE_SELF, &r); return r.ru_minflt; }
static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec / 1e9; }
int main(void)
{
    long ps = sysconf(_SC_PAGESIZE); size_t pages = 51200;       /* 200 MB */
    size_t *perm = malloc(pages * sizeof *perm);
    for (size_t i = 0; i < pages; i++) perm[i] = i;
    srand(3); for (size_t i = pages - 1; i > 0; i--) { size_t j = (size_t)rand() % (i + 1), t = perm[i]; perm[i] = perm[j]; perm[j] = t; }
    for (int order = 0; order < 2; order++) {
        char *m = mmap(NULL, pages * ps, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        long f0 = minflt(); double t0 = now();
        for (size_t i = 0; i < pages; i++) m[(order ? perm[i] : i) * ps] = 1;
        double t1 = now(); long f1 = minflt();
        long sum = 0; for (int rep = 0; rep < 5; rep++) for (size_t i = 0; i < pages; i++) sum += m[(order ? perm[i] : i) * ps];
        double t2 = now(); long f2 = minflt();
        printf("%-10s first touch: %6ld faults in %6.1f ms (%.2f us/fault); 5 more passes: %ld faults, %6.1f ms (checksum %ld)\n", order ? "random" : "sequential", f1 - f0, (t1 - t0) * 1e3, (t1 - t0) * 1e6 / (f1 - f0), f2 - f1, (t2 - t1) * 1e3, sum);
        munmap(m, pages * ps);
    }
    return 0;
}
