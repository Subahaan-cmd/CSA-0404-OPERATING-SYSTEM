/* Q7 Stage 3: worst-case delay of a time-critical loop with and without locked memory.
   Each iteration reads/updates one new 4 KB page of the controller's data (anonymous or file-backed, evicted from cache). */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/resource.h>
#define PAGES 8192                                  /* 32 MB of controller data */
static double now_us(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec * 1e6 + t.tv_nsec / 1e3; }
static int cmp(const void *a, const void *b) { double x = *(const double *)a, y = *(const double *)b; return x < y ? -1 : x > y; }
static long majflt(void) { struct rusage r; getrusage(RUSAGE_SELF, &r); return r.ru_majflt; }
static long minflt(void) { struct rusage r; getrusage(RUSAGE_SELF, &r); return r.ru_minflt; }
static void report(const char *name, double *d, int n, long mn, long mj)
{
    double sum = 0; for (int i = 0; i < n; i++) sum += d[i];
    qsort(d, n, sizeof *d, cmp);
    printf("%-34s mean %8.2f us  p99 %8.2f us  p99.9 %8.2f us  WORST %9.2f us   minor faults %5ld  major faults %4ld\n", name, sum / n, d[(int)(n * 0.99)], d[(int)(n * 0.999)], d[n - 1], mn, mj);
}
int main(void)
{
    long ps = 4096; static double d[PAGES];
    /* ---- (1) anonymous memory, first touch of each page */
    for (int lock = 0; lock < 2; lock++) {
        char *m = mmap(NULL, PAGES * ps, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        int ok = 1;
        if (lock) { if (mlock(m, PAGES * ps)) { perror("mlock"); ok = 0; } else for (int i = 0; i < PAGES; i++) m[i * ps] = 0; }   /* lock and pre-fault before the loop starts */
        long a = minflt(), b = majflt();
        for (int i = 0; i < PAGES; i++) { double t0 = now_us(); m[i * ps] = (char)i; d[i] = now_us() - t0; }
        report(lock ? (ok ? "anonymous data, mlock + pre-fault" : "anonymous data, mlock FAILED") : "anonymous data, no locking", d, PAGES, minflt() - a, majflt() - b);
        munmap(m, PAGES * ps);
    }
    /* ---- (2) file-backed data whose pages have been evicted from the page cache (major faults) */
    int fd = open("ctl.dat", O_RDWR | O_CREAT | O_TRUNC, 0600); char *z = calloc(1, 1 << 20); for (int i = 0; i < PAGES * ps / (1 << 20); i++) if (write(fd, z, 1 << 20) < 0) return 1;
    fsync(fd);
    for (int lock = 0; lock < 2; lock++) {
        posix_fadvise(fd, 0, 0, POSIX_FADV_DONTNEED);           /* evict the file from the page cache */
        char *m = mmap(NULL, PAGES * ps, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (lock) { if (mlock(m, PAGES * ps)) perror("mlock"); }            /* mlock reads every page in now, before the critical loop */
        long a = minflt(), b = majflt();
        for (int i = 0; i < PAGES; i++) { double t0 = now_us(); volatile char c = m[(size_t)i * ps]; (void)c; d[i] = now_us() - t0; }
        report(lock ? "file-backed (evicted), mlock" : "file-backed (evicted), no locking", d, PAGES, minflt() - a, majflt() - b);
        munmap(m, PAGES * ps);
    }
    unlink("ctl.dat");
    return 0;
}
