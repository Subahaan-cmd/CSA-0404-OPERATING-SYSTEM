/* Task 7/10: time to create and reap 1000 processes (fork, vfork-like exec-less) and 1000 threads. */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <spawn.h>
#include <sys/wait.h>
#include <sys/mman.h>
extern char **environ;
static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec / 1e9; }
static void *thr(void *a) { (void)a; return NULL; }
int main(int argc, char **argv)
{
    size_t heap_mb = argc > 1 ? (size_t)atoi(argv[1]) : 0;
    char *heap = NULL;
    if (heap_mb) { heap = malloc(heap_mb << 20); for (size_t i = 0; i < (heap_mb << 20); i += 4096) heap[i] = 1; }
    const int N = 1000;
    double t0 = now();
    for (int i = 0; i < N; i++) { pid_t p = fork(); if (p == 0) _exit(0); waitpid(p, NULL, 0); }
    double t1 = now();
    for (int i = 0; i < N; i++) { pthread_t t; pthread_create(&t, NULL, thr, NULL); pthread_join(t, NULL); }
    double t2 = now();
    for (int i = 0; i < N; i++) { pid_t p; char *av[] = {"true", NULL}; posix_spawnp(&p, "true", NULL, NULL, av, environ); waitpid(p, NULL, 0); }
    double t3 = now();
    for (int i = 0; i < N; i++) { pid_t p = fork(); if (p == 0) { execlp("true", "true", (char *)NULL); _exit(127); } waitpid(p, NULL, 0); }
    double t4 = now();
    printf("parent heap touched: %zu MB\n", heap_mb);
    printf("1000 x fork+exit+wait      : %8.1f ms  (%6.1f us each)\n", (t1 - t0) * 1e3, (t1 - t0) * 1e6 / N);
    printf("1000 x thread create+join  : %8.1f ms  (%6.1f us each)\n", (t2 - t1) * 1e3, (t2 - t1) * 1e6 / N);
    printf("1000 x posix_spawn(true)   : %8.1f ms  (%6.1f us each)\n", (t3 - t2) * 1e3, (t3 - t2) * 1e6 / N);
    printf("1000 x fork+exec(true)     : %8.1f ms  (%6.1f us each)\n", (t4 - t3) * 1e3, (t4 - t3) * 1e6 / N);
    return 0;
}
