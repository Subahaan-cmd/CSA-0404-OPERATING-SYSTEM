/* Q4: time to transfer 1, 10 and 100 MB from one process to another through (1) a pipe, (2) a socketpair,
   (3) a POSIX message queue and (4) shared memory with semaphores. */
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <mqueue.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/wait.h>
#define CH 65536
static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec / 1e9; }
static double via_fd(int fd[2], size_t total)
{
    char *buf = malloc(CH); memset(buf, 7, CH);
    double t0 = now(); pid_t p = fork();
    if (p == 0) { close(fd[1]); size_t got = 0; ssize_t n; while ((n = read(fd[0], buf, CH)) > 0) got += n; _exit(got == total ? 0 : 1); }
    close(fd[0]); for (size_t s = 0; s < total; s += CH) if (write(fd[1], buf, CH) != CH) break;
    close(fd[1]); int st; waitpid(p, &st, 0); free(buf); return WEXITSTATUS(st) ? -1 : now() - t0;
}
static double via_mq(size_t total)
{
    struct mq_attr at = {0, 10, 8192, 0}; mq_unlink("/q4bench");
    mqd_t q = mq_open("/q4bench", O_CREAT | O_RDWR, 0600, &at);
    if (q == (mqd_t)-1) return -1;
    char *buf = malloc(8192); memset(buf, 7, 8192);
    double t0 = now(); pid_t p = fork();
    if (p == 0) { size_t got = 0; while (got < total) { ssize_t n = mq_receive(q, buf, 8192, NULL); if (n <= 0) break; got += n; } _exit(got == total ? 0 : 1); }
    for (size_t s = 0; s < total; s += 8192) mq_send(q, buf, 8192, 0);
    int st; waitpid(p, &st, 0); mq_close(q); mq_unlink("/q4bench"); free(buf); return WEXITSTATUS(st) ? -1 : now() - t0;
}
struct ring { sem_t full[8], empty[8]; char data[8][CH]; };
static double via_shm(size_t total)
{
    struct ring *r = mmap(NULL, sizeof *r, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for (int i = 0; i < 8; i++) { sem_init(&r->full[i], 1, 0); sem_init(&r->empty[i], 1, 1); }
    size_t chunks = total / CH; double t0 = now(); pid_t p = fork();
    if (p == 0) { unsigned long sum = 0; for (size_t c = 0; c < chunks; c++) { int i = c % 8; sem_wait(&r->full[i]); sum += (unsigned char)r->data[i][0]; sem_post(&r->empty[i]); } _exit(sum == 7 * chunks ? 0 : 1); }
    for (size_t c = 0; c < chunks; c++) { int i = c % 8; sem_wait(&r->empty[i]); memset(r->data[i], 7, CH); sem_post(&r->full[i]); }
    int st; waitpid(p, &st, 0); munmap(r, sizeof *r); return WEXITSTATUS(st) ? -1 : now() - t0;
}
int main(void)
{
    size_t sizes[] = {1, 10, 100};
    printf("%-8s %12s %12s %12s %16s\n", "size", "pipe (s)", "socketpair", "msg queue", "shared mem+sem");
    for (int k = 0; k < 3; k++) {
        size_t total = sizes[k] * 1024 * 1024; int a[2], b[2];
        if (pipe(a)) return 1; double tp = via_fd(a, total);
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, b)) return 1; double ts = via_fd(b, total);
        double tq = via_mq(total), tm = via_shm(total);
        printf("%4zu MB  %12.4f %12.4f ", sizes[k], tp, ts);
        if (tq < 0) printf("%12s ", "n/a"); else printf("%12.4f ", tq);
        printf("%16.4f\n", tm);
    }
    return 0;
}
