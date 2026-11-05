/* Q5: dining philosophers with semaphores. Deadlock is avoided by letting at most 4 philosophers sit (counting semaphore). */
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define N 5
static sem_t fork_[N], room;
static int meals[N];
static void *phil(void *a)
{
    int i = (int)(long)a;
    for (int m = 0; m < 1000; m++) {
        sem_wait(&room);                    /* at most N-1 philosophers try to eat */
        sem_wait(&fork_[i]); sem_wait(&fork_[(i + 1) % N]);
        meals[i]++;                         /* eat */
        sem_post(&fork_[(i + 1) % N]); sem_post(&fork_[i]);
        sem_post(&room);
    }
    return NULL;
}
int main(void)
{
    pthread_t t[N];
    for (int i = 0; i < N; i++) sem_init(&fork_[i], 0, 1);
    sem_init(&room, 0, N - 1);
    for (long i = 0; i < N; i++) pthread_create(&t[i], NULL, phil, (void *)i);
    for (int i = 0; i < N; i++) pthread_join(t[i], NULL);
    printf("meals eaten:"); for (int i = 0; i < N; i++) printf(" %d", meals[i]); printf("  -> no deadlock, no starvation\n");
    return 0;
}
