#define _DEFAULT_SOURCE
/* Q1: create N child processes (one per "student session") and observe them with ps and /proc. */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int n = argc > 1 ? atoi(argv[1]) : 5;
    pid_t kids[64];
    if (n > 64) n = 64;
    for (int i = 0; i < n; i++) {
        pid_t p = fork();
        if (p == 0) { sleep(3); _exit(i); }     /* the child "serves a session": it waits */
        kids[i] = p;
    }
    printf("parent %d created %d children; first child is %d\n", getpid(), n, kids[0]);
    fflush(stdout);
    usleep(200000);
    char cmd[200];
    snprintf(cmd, sizeof cmd, "ps -o pid,ppid,stat,vsz,rss,nlwp,comm --ppid %d | head -8", getpid());
    if (system(cmd)) {}
    printf("\n/proc/%d/status (excerpt = the PCB as seen by Linux):\n", kids[0]);
    fflush(stdout);
    snprintf(cmd, sizeof cmd, "grep -E '^(Name|State|Pid|PPid|Uid|VmSize|VmRSS|Threads|voluntary_ctxt_switches|nonvoluntary_ctxt_switches)' /proc/%d/status", kids[0]);
    if (system(cmd)) {}
    printf("\n/proc/%d/stat fields: ", kids[0]); fflush(stdout);
    snprintf(cmd, sizeof cmd, "cut -d' ' -f1-8,14-15,19-20,23 /proc/%d/stat", kids[0]);
    if (system(cmd)) {}
    int st, total = 0;
    for (int i = 0; i < n; i++) { waitpid(kids[i], &st, 0); total += WEXITSTATUS(st); }
    printf("\nall children terminated; sum of exit codes = %d\n", total);
    return 0;
}
