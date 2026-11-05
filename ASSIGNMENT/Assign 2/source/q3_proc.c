/* Q3 alternative: 64 frames processed by 4 worker PROCESSES; results combined through pipes (IPC). */
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#define CAMERAS 64
static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec / 1e9; }
static volatile unsigned long sink;
static long process_frame(int id) { unsigned long h = 1469598103934665603UL + id; for (long i = 0; i < 4000000; i++) h = (h ^ (unsigned long)i) * 1099511628211UL; sink = h; return 1; }
int main(void)
{
    int W = 4, fd[4][2]; pid_t pid[4];
    double t0 = now();
    for (int w = 0; w < W; w++) {
        if (pipe(fd[w])) return 1;
        pid[w] = fork();
        if (pid[w] == 0) { close(fd[w][0]); long found = 0; for (int i = w; i < CAMERAS; i += W) found += process_frame(i); if (write(fd[w][1], &found, sizeof found) < 0) _exit(1); _exit(0); }
        close(fd[w][1]);
    }
    long total = 0;
    for (int w = 0; w < W; w++) { long v; if (read(fd[w][0], &v, sizeof v) == sizeof v) total += v; waitpid(pid[w], NULL, 0); }
    printf("4 worker processes: %ld frames in %.3f s (results combined through pipes)\n", total, now() - t0);
    return 0;
}
