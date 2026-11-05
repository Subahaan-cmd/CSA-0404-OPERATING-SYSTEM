#define _DEFAULT_SOURCE
/* Q1: memory cost per idle session when it is a process and when it is a thread (MemAvailable difference). */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
static long avail_kb(void) { FILE *f = fopen("/proc/meminfo", "r"); char l[128]; long v = 0; while (fgets(l, sizeof l, f)) if (!strncmp(l, "MemAvailable:", 13)) { v = atol(l + 13); break; } fclose(f); return v; }
static void *idle(void *x) { (void)x; char buf[2048]; memset(buf, 1, sizeof buf); sleep(5); return buf[0] ? NULL : NULL; }
int main(int argc, char **argv)
{
    int n = 300, proc = argc > 1 && argv[1][0] == 'p';
    long before = avail_kb();
    pid_t kids[400]; pthread_t th[400];
    for (int i = 0; i < n; i++) {
        if (proc) { pid_t p = fork(); if (p == 0) { char buf[2048]; memset(buf, 1, sizeof buf); sleep(5); _exit(buf[0] ? 0 : 1); } kids[i] = p; }
        else { pthread_attr_t at; pthread_attr_init(&at); pthread_attr_setstacksize(&at, 256 * 1024); pthread_create(&th[i], &at, idle, NULL); }
    }
    usleep(1500000);
    long after = avail_kb();
    printf("%s: %d idle sessions use %ld KB -> %.1f KB each\n", proc ? "processes" : "threads  ", n, before - after, (double)(before - after) / n);
    fflush(stdout);
    for (int i = 0; i < n; i++) { if (proc) { kill(kids[i], SIGKILL); waitpid(kids[i], NULL, 0); } else pthread_detach(th[i]); }
    return 0;
}
