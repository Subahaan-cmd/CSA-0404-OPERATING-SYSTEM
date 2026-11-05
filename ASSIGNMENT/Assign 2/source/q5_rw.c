/* Q5: readers-writers (readers preference) with two semaphores; 4 readers and 2 writers on one shared record. */
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
static sem_t rmutex, wrt;
static int readcount = 0, record = 0, bad = 0;
static void *reader(void *a)
{
    (void)a;
    for (int i = 0; i < 50000; i++) {
        sem_wait(&rmutex); if (++readcount == 1) sem_wait(&wrt); sem_post(&rmutex);     /* first reader locks out writers */
        int x = record, y = record; if (x != y || (x & 1)) bad++;                       /* record must never be seen half-updated */
        sem_wait(&rmutex); if (--readcount == 0) sem_post(&wrt); sem_post(&rmutex);     /* last reader lets writers in */
    }
    return NULL;
}
static void *writer(void *a)
{
    (void)a;
    for (int i = 0; i < 5000; i++) { sem_wait(&wrt); record++; record++; sem_post(&wrt); }
    return NULL;
}
int main(void)
{
    sem_init(&rmutex, 0, 1); sem_init(&wrt, 0, 1);
    pthread_t r[4], w[2];
    for (int i = 0; i < 4; i++) pthread_create(&r[i], NULL, reader, NULL);
    for (int i = 0; i < 2; i++) pthread_create(&w[i], NULL, writer, NULL);
    for (int i = 0; i < 4; i++) pthread_join(r[i], NULL);
    for (int i = 0; i < 2; i++) pthread_join(w[i], NULL);
    printf("final record = %d (expected 20000), inconsistent reads = %d\n", record, bad);
    return 0;
}
