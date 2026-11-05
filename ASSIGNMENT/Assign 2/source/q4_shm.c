/* Q4 alternative: the same readings through shared memory protected by two POSIX semaphores. */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>
struct shared { sem_t full, empty; int slot; int done; };
int main(void)
{
    struct shared *s = mmap(NULL, sizeof *s, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(&s->full, 1, 0); sem_init(&s->empty, 1, 1); s->done = 0;      /* pshared = 1: shared between processes */
    pid_t pid = fork();
    if (pid == 0) {
        int count = 0, sum = 0, max = 0;
        for (;;) {
            sem_wait(&s->full);                      /* wait until the reader has stored a value */
            if (s->done) break;
            int r = s->slot; count++; sum += r; if (r > max) max = r;
            sem_post(&s->empty);                     /* slot is free again */
        }
        printf("analyser: %d readings, average %.1f, max %d\n", count, count ? (double)sum / count : 0.0, max);
        fflush(stdout); _exit(0);
    }
    int readings[] = {31, 33, 36, 34, 41, 38};
    for (int i = 0; i < 6; i++) { sem_wait(&s->empty); s->slot = readings[i]; sem_post(&s->full); }
    sem_wait(&s->empty); s->done = 1; sem_post(&s->full);        /* end-of-data marker */
    waitpid(pid, NULL, 0);
    printf("reader: done\n");
    return 0;
}
