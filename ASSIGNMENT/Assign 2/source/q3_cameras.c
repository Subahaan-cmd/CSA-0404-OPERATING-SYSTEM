/* Q3: process 64 camera frames: sequentially, one thread per camera, and with pools of 4, 8 and 16 threads.
   Each frame = (optional) I/O wait for the camera + CPU work for number-plate recognition (simulated). */
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdatomic.h>
#define CAMERAS 64
static int io_ms;                       /* simulated camera/network wait per frame */
static long work_iters = 4000000;       /* simulated recognition work per frame    */
static atomic_long plates_found;        /* shared result counter (protected by atomics) */
static atomic_int next_frame;
static volatile unsigned long sink;   /* prevents the compiler from deleting the work loop */
static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec / 1e9; }
static void process_frame(int id)
{
    if (io_ms) { struct timespec t = {0, io_ms * 1000000L}; nanosleep(&t, NULL); }
    unsigned long h = 1469598103934665603UL + id;
    for (long i = 0; i < work_iters; i++) h = (h ^ (unsigned long)i) * 1099511628211UL;   /* CPU-bound loop */
    sink = h;                                   /* use the result so that the loop is really executed */
    atomic_fetch_add(&plates_found, 1);         /* one more frame done: shared counter, atomic update */
}
static void *one(void *a) { process_frame((int)(long)a); return NULL; }
static void *pool_worker(void *a) { (void)a; int i; while ((i = atomic_fetch_add(&next_frame, 1)) < CAMERAS) process_frame(i); return NULL; }
static double run_seq(void) { double t0 = now(); for (int i = 0; i < CAMERAS; i++) process_frame(i); return now() - t0; }
static double run_per_camera(void) { pthread_t t[CAMERAS]; double t0 = now(); for (long i = 0; i < CAMERAS; i++) pthread_create(&t[i], NULL, one, (void *)i); for (int i = 0; i < CAMERAS; i++) pthread_join(t[i], NULL); return now() - t0; }
static double run_pool(int n) { pthread_t t[64]; atomic_store(&next_frame, 0); double t0 = now(); for (int i = 0; i < n; i++) pthread_create(&t[i], NULL, pool_worker, NULL); for (int i = 0; i < n; i++) pthread_join(t[i], NULL); return now() - t0; }
int main(void)
{
    int ios[] = {0, 5};
    for (int k = 0; k < 2; k++) {
        io_ms = ios[k];
        printf("\nWorkload: %s\n%-26s %9s %9s\n", io_ms ? "5 ms I/O wait + CPU work per frame" : "CPU work only", "method", "time (s)", "speed-up");
        double seq = run_seq();
        printf("%-26s %9.3f %9.2f\n", "sequential (1 thread)", seq, 1.0);
        double d;
        d = run_pool(4);  printf("%-26s %9.3f %9.2f\n", "pool of 4 threads", d, seq / d);
        d = run_pool(8);  printf("%-26s %9.3f %9.2f\n", "pool of 8 threads", d, seq / d);
        d = run_pool(16); printf("%-26s %9.3f %9.2f\n", "pool of 16 threads", d, seq / d);
        d = run_per_camera(); printf("%-26s %9.3f %9.2f\n", "one thread per camera (64)", d, seq / d);
    }
    printf("\nframes processed in all runs: %ld (expected %d)\n", (long)atomic_load(&plates_found), CAMERAS * 10);
    return 0;
}
