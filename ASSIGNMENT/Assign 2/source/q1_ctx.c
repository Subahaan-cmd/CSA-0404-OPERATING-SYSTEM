/* Q1/Q3: cost of a context switch between two processes and between two threads (ping-pong through pipes). */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/wait.h>
#define ROUNDS 100000
static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec / 1e9; }
static int a[2], b[2];
static void *echo(void *x) { (void)x; char c; for (int i = 0; i < ROUNDS; i++) { if (read(a[0], &c, 1) != 1) break; if (write(b[1], &c, 1) != 1) break; } return NULL; }
int main(void)
{
    char c = 'x';
    if (pipe(a) || pipe(b)) return 1;
    pid_t p = fork();
    if (p == 0) { echo(NULL); _exit(0); }
    double t0 = now();
    for (int i = 0; i < ROUNDS; i++) { if (write(a[1], &c, 1) != 1 || read(b[0], &c, 1) != 1) break; }
    double t1 = now(); waitpid(p, NULL, 0);
    printf("processes: %d round trips in %.3f s -> %.2f us per switch\n", ROUNDS, t1 - t0, (t1 - t0) * 1e6 / (2.0 * ROUNDS));
    pipe(a); pipe(b);
    pthread_t th; pthread_create(&th, NULL, echo, NULL);
    t0 = now();
    for (int i = 0; i < ROUNDS; i++) { if (write(a[1], &c, 1) != 1 || read(b[0], &c, 1) != 1) break; }
    t1 = now(); pthread_join(th, NULL);
    printf("threads  : %d round trips in %.3f s -> %.2f us per switch\n", ROUNDS, t1 - t0, (t1 - t0) * 1e6 / (2.0 * ROUNDS));
    return 0;
}
