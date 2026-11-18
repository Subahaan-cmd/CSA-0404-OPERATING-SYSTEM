/* syncbench.c - cost of durability on ext4: 2,000 appends of 4 KB with different sync policies */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#define N 2000
static double now(void){ struct timespec t; clock_gettime(CLOCK_MONOTONIC,&t); return t.tv_sec*1e3+t.tv_nsec/1e6; }
static void run(const char *label, int mode)
{
    char buf[4096]; memset(buf,'x',sizeof buf);
    int fd = open("bench.dat", O_WRONLY|O_CREAT|O_TRUNC, 0644);
    double t0 = now();
    for (int i = 0; i < N; i++) {
        if (write(fd, buf, sizeof buf) != (ssize_t)sizeof buf) { perror("write"); return; }
        if (mode == 1) fsync(fd);
        if (mode == 2) fdatasync(fd);
    }
    if (mode == 0) fsync(fd);              /* one flush at the end so all data is durable */
    double t1 = now();
    close(fd); unlink("bench.dat");
    printf("%-34s total %8.1f ms   per write %8.3f ms\n", label, t1 - t0, (t1 - t0) / N);
}
int main(void)
{
    run("write, one fsync at the end", 0);
    run("write + fsync after every write", 1);
    run("write + fdatasync after every write", 2);
    return 0;
}
