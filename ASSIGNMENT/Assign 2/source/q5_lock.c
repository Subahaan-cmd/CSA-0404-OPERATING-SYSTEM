/* Q5: throughput of (a) global / per-train / per-coach locking of a seat-booking table and
   (b) mutex, spin lock and C11 atomic counter with 1, 4 and 16 threads. */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>
#define TRAINS 20
#define COACHES 10
#define SEATS 72
#define OPS 400000
static int seat[TRAINS][COACHES][SEATS];
static pthread_mutex_t global = PTHREAD_MUTEX_INITIALIZER, train_l[TRAINS], coach_l[TRAINS][COACHES];
static int mode, nthreads;
static atomic_long booked;
static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec / 1e9; }
static void work(void) { for (volatile int i = 0; i < 40; i++); }          /* ~ the cost of checking and updating the seat */
static void *agent(void *arg)
{
    unsigned seed = (unsigned)(long)arg * 7919u + 1;
    for (int k = 0; k < OPS / nthreads; k++) {
        int t = rand_r(&seed) % TRAINS, c = rand_r(&seed) % COACHES, s = rand_r(&seed) % SEATS;
        pthread_mutex_t *m = mode == 0 ? &global : mode == 1 ? &train_l[t] : &coach_l[t][c];
        pthread_mutex_lock(m);
        if (!seat[t][c][s]) { work(); seat[t][c][s] = 1; atomic_fetch_add(&booked, 1); }
        pthread_mutex_unlock(m);
    }
    return NULL;
}
static double run_booking(int m, int n)
{
    mode = m; nthreads = n; pthread_t th[16];
    for (int t = 0; t < TRAINS; t++) for (int c = 0; c < COACHES; c++) for (int s = 0; s < SEATS; s++) seat[t][c][s] = 0;
    double t0 = now();
    for (long i = 0; i < n; i++) pthread_create(&th[i], NULL, agent, (void *)i);
    for (int i = 0; i < n; i++) pthread_join(th[i], NULL);
    return now() - t0;
}
/* ---- counter protected by mutex / spin lock / atomic */
#define UPD 1000000L
static long cnt; static pthread_mutex_t cm = PTHREAD_MUTEX_INITIALIZER; static atomic_flag sp = ATOMIC_FLAG_INIT; static atomic_long ac;
static int cmode; static int cn;
static void *upd(void *a)
{
    (void)a;
    for (long i = 0; i < UPD / cn; i++) {
        if (cmode == 0) { pthread_mutex_lock(&cm); cnt++; pthread_mutex_unlock(&cm); }
        else if (cmode == 1) { while (atomic_flag_test_and_set_explicit(&sp, memory_order_acquire)) ; cnt++; atomic_flag_clear_explicit(&sp, memory_order_release); }
        else atomic_fetch_add(&ac, 1);
    }
    return NULL;
}
static double run_counter(int m, int n)
{
    cmode = m; cn = n; cnt = 0; atomic_store(&ac, 0); pthread_t th[16]; double t0 = now();
    for (int i = 0; i < n; i++) pthread_create(&th[i], NULL, upd, NULL);
    for (int i = 0; i < n; i++) pthread_join(th[i], NULL);
    return now() - t0;
}
int main(void)
{
    for (int t = 0; t < TRAINS; t++) { pthread_mutex_init(&train_l[t], NULL); for (int c = 0; c < COACHES; c++) pthread_mutex_init(&coach_l[t][c], NULL); }
    const char *nm[] = {"one global lock", "one lock per train (20)", "one lock per coach (200)"};
    printf("Booking throughput (%d booking attempts, bookings per second):\n%-26s %12s %12s %12s\n", OPS, "locking scheme", "1 thread", "4 threads", "16 threads");
    int th[] = {1, 4, 16};
    for (int m = 0; m < 3; m++) {
        printf("%-26s", nm[m]);
        for (int k = 0; k < 3; k++) printf(" %12.0f", OPS / run_booking(m, th[k]));
        printf("\n");
    }
    printf("\nCounter updates (%ld increments in total), time per update in nanoseconds:\n%-14s %10s %10s %10s\n", UPD, "method", "1 thread", "4 threads", "16 threads");
    const char *cm_[] = {"mutex", "spin lock", "atomic add"};
    for (int m = 0; m < 3; m++) {
        printf("%-14s", cm_[m]);
        for (int k = 0; k < 3; k++) printf(" %10.1f", run_counter(m, th[k]) * 1e9 / UPD);
        printf("\n");
    }
    return 0;
}
