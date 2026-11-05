/* Trigger a segmentation fault and show what the OS reports to the process (SIGSEGV handler). */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
static void handler(int sig, siginfo_t *si, void *ctx)
{
    (void)ctx;
    char msg[128];
    int n = snprintf(msg, sizeof msg, "OS delivered signal %d (SIGSEGV) for address %p, si_code=%s\n", sig, si->si_addr,
                     si->si_code == SEGV_MAPERR ? "SEGV_MAPERR (address not mapped)" : "SEGV_ACCERR (permission violation)");
    if (write(1, msg, n) < 0) {}
    _exit(139);
}
int main(int argc, char **argv)
{
    struct sigaction sa; memset(&sa, 0, sizeof sa); sa.sa_sigaction = handler; sa.sa_flags = SA_SIGINFO; sigaction(SIGSEGV, &sa, NULL);
    if (argc > 1) { char *p = (char *)"constant string"; printf("writing into read-only code/data segment...\n"); fflush(stdout); p[0] = 'X'; }
    else { volatile int *q = (int *)0x10; printf("reading address 0x10 (outside any segment)...\n"); fflush(stdout); return *q; }
    return 0;
}
