/* Q3: two threads update a shared counter 20 million times each: unprotected, with a mutex, and with an atomic. */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>
#define N 20000000L
static volatile long plain;
static long guarded; static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static atomic_long atom;
static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec / 1e9; }
static void *f_plain(void *a) { (void)a; for (long i = 0; i < N; i++) plain = plain + 1; return NULL; }
static void *f_mutex(void *a) { (void)a; for (long i = 0; i < N; i++) { pthread_mutex_lock(&m); guarded++; pthread_mutex_unlock(&m); } return NULL; }
static void *f_atomic(void *a) { (void)a; for (long i = 0; i < N; i++) atomic_fetch_add(&atom, 1); return NULL; }
static double go(void *(*f)(void *)) { pthread_t a, b; double t0 = now(); pthread_create(&a, NULL, f, NULL); pthread_create(&b, NULL, f, NULL); pthread_join(a, NULL); pthread_join(b, NULL); return now() - t0; }
int main(void)
{
    double t;
    t = go(f_plain);  printf("unprotected : counter = %ld (expected %ld), lost %ld updates, %.3f s\n", plain, 2 * N, 2 * N - plain, t);
    t = go(f_mutex);  printf("mutex       : counter = %ld (expected %ld), %.3f s\n", guarded, 2 * N, t);
    t = go(f_atomic); printf("atomic add  : counter = %ld (expected %ld), %.3f s\n", (long)atom, 2 * N, t);
    return 0;
}
