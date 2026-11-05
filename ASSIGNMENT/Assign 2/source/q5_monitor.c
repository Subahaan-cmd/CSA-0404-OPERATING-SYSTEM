/* Q5: the same bounded buffer written as a MONITOR: one mutex + two condition variables. Easier to get right. */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <pthread.h>
#define SIZE 5
#define PER 20000
static int buf[SIZE], in, out, count;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t not_full = PTHREAD_COND_INITIALIZER, not_empty = PTHREAD_COND_INITIALIZER;
static long sum;
static void put(int v) { pthread_mutex_lock(&m); while (count == SIZE) pthread_cond_wait(&not_full, &m); buf[in] = v; in = (in + 1) % SIZE; count++; pthread_cond_signal(&not_empty); pthread_mutex_unlock(&m); }
static int get(void) { pthread_mutex_lock(&m); while (count == 0) pthread_cond_wait(&not_empty, &m); int v = buf[out]; out = (out + 1) % SIZE; count--; pthread_cond_signal(&not_full); pthread_mutex_unlock(&m); return v; }
static void *prod(void *a) { (void)a; for (int i = 1; i <= PER; i++) put(i); return NULL; }
static void *cons(void *a) { (void)a; long s = 0; for (int i = 0; i < PER; i++) s += get(); pthread_mutex_lock(&m); sum += s; pthread_mutex_unlock(&m); return NULL; }
int main(void)
{
    pthread_t p[4], c[4];
    for (int i = 0; i < 4; i++) { pthread_create(&p[i], NULL, prod, NULL); pthread_create(&c[i], NULL, cons, NULL); }
    for (int i = 0; i < 4; i++) { pthread_join(p[i], NULL); pthread_join(c[i], NULL); }
    printf("monitor bounded buffer, 4+4 threads: sum = %ld (expected %ld)\n", sum, 4L * PER * (PER + 1) / 2);
    return 0;
}
