/* Copy-on-write: fork() a process holding 100 MB, then measure the child's faults and private memory before/after writing. */
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/resource.h>
static long minflt(void) { struct rusage r; getrusage(RUSAGE_SELF, &r); return r.ru_minflt; }
static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec / 1e9; }
static long field(const char *name) { FILE *f = fopen("/proc/self/smaps_rollup", "r"); char l[128]; long v = -1; size_t n = strlen(name); while (fgets(l, sizeof l, f)) if (!strncmp(l, name, n)) { v = atol(l + n + 1); break; } fclose(f); return v; }
int main(void)
{
    size_t sz = 100u << 20; char *b = malloc(sz); memset(b, 1, sz);
    printf("parent: Rss %ld KB, Private_Dirty %ld KB\n", field("Rss"), field("Private_Dirty")); fflush(stdout);
    double t0 = now(); pid_t p = fork();
    if (p == 0) {
        double t1 = now();
        printf("child  right after fork (fork took %.2f ms): Private_Dirty %ld KB, Shared %ld KB\n", (t1 - t0) * 1e3, field("Private_Dirty"), field("Shared_Dirty") + field("Shared_Clean"));
        long f0 = minflt(); double w0 = now();
        for (size_t i = 0; i < sz / 2; i += 4096) b[i] = 2;              /* write to half of the pages */
        double w1 = now();
        printf("child  after writing 50 MB: %ld copy-on-write faults in %.1f ms, Private_Dirty %ld KB\n", minflt() - f0, (w1 - w0) * 1e3, field("Private_Dirty"));
        fflush(stdout); _exit(0);
    }
    waitpid(p, NULL, 0);
    return 0;
}
