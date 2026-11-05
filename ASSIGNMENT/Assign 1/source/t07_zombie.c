#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
/* Creates a zombie (child exits, parent has not yet called wait) and shows its state in /proc. */
int main(void)
{
    pid_t pid = fork();
    if (pid == 0) _exit(0);
    sleep(1);                                   /* child is now a zombie */
    char cmd[64];
    snprintf(cmd, sizeof cmd, "grep -E 'Name|State' /proc/%d/status", pid);
    printf("child %d after exit, before wait():\n", pid); fflush(stdout);
    if (system(cmd)) {}
    waitpid(pid, NULL, 0);                      /* reaping removes the zombie */
    printf("after waitpid(): "); fflush(stdout);
    snprintf(cmd, sizeof cmd, "test -e /proc/%d && echo still there || echo entry removed", pid);
    if (system(cmd)) {}
    return 0;
}
