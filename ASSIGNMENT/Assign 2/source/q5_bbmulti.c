/* Q5 test: bounded buffer with K producers and K consumers. Every item is tagged and counted exactly once.
   Compile with -DNOLOCK to remove the mutex and see the failure. */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sched.h>
#define SIZE 5
#define PER 20000
static int buffer[SIZE], in = 0, out = 0, K = 4;
static sem_t empty, full;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t sumlock = PTHREAD_MUTEX_INITIALIZER;
static long consumed_sum = 0, consumed_count = 0;
#ifdef NOLOCK
#define LOCK()
#define UNLOCK()
#else
#define LOCK() pthread_mutex_lock(&lock)
#define UNLOCK() pthread_mutex_unlock(&lock)
#endif
static void *producer(void *arg)
{
    (void)arg;
    for (int item = 1; item <= PER; item++) {
        sem_wait(&empty); LOCK();
        buffer[in] = item; if (item % 97 == 0) sched_yield(); in = (in + 1) % SIZE;
        UNLOCK(); sem_post(&full);
    }
    return NULL;
}
static void *consumer(void *arg)
{
    (void)arg;
    for (int k = 0; k < PER; k++) {
        sem_wait(&full); LOCK();
        int item = buffer[out]; if (k % 89 == 0) sched_yield(); out = (out + 1) % SIZE;
        UNLOCK(); sem_post(&empty);
        pthread_mutex_lock(&sumlock); consumed_sum += item; consumed_count++; pthread_mutex_unlock(&sumlock);
    }
    return NULL;
}
int main(int argc, char **argv)
{
    if (argc > 1) K = atoi(argv[1]);
    sem_init(&empty, 0, SIZE); sem_init(&full, 0, 0);
    pthread_t p[16], c[16];
    for (int i = 0; i < K; i++) { pthread_create(&p[i], NULL, producer, NULL); pthread_create(&c[i], NULL, consumer, NULL); }
    for (int i = 0; i < K; i++) { pthread_join(p[i], NULL); pthread_join(c[i], NULL); }
    long expect = (long)K * PER * (PER + 1) / 2;
    printf("%d producers/consumers: consumed %ld items, sum = %ld (expected %ld) -> %s\n", K, consumed_count, consumed_sum, expect,
           consumed_sum == expect ? "CORRECT" : "WRONG");
    return 0;
}
