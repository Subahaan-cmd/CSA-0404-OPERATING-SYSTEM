/* A tiny strace: prints every system call (number and name) made by a child, and a -c style summary.
   Usage: ./mystrace [-c] program args...   (x86-64 Linux only; uses ptrace) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/syscall.h>

static const char *name(long n)
{
    switch (n) {
    case SYS_read: return "read"; case SYS_write: return "write"; case SYS_open: return "open";
    case SYS_openat: return "openat"; case SYS_close: return "close"; case SYS_fstat: return "fstat";
    case SYS_newfstatat: return "newfstatat"; case SYS_mmap: return "mmap"; case SYS_munmap: return "munmap";
    case SYS_mprotect: return "mprotect"; case SYS_brk: return "brk"; case SYS_execve: return "execve";
    case SYS_exit_group: return "exit_group"; case SYS_access: return "access"; case SYS_pread64: return "pread64";
    case SYS_arch_prctl: return "arch_prctl"; case SYS_set_tid_address: return "set_tid_address";
    case SYS_set_robust_list: return "set_robust_list"; case SYS_rseq: return "rseq"; case SYS_prlimit64: return "prlimit64";
    case SYS_getrandom: return "getrandom"; case SYS_unlink: return "unlink"; case SYS_lseek: return "lseek";
    case SYS_ioctl: return "ioctl"; case SYS_fork: return "fork"; case SYS_clone: return "clone";
    case SYS_wait4: return "wait4"; case SYS_getpid: return "getpid"; case SYS_readlink: return "readlink";
    default: return NULL;
    }
}
int main(int argc, char **argv)
{
    int summary = argc > 2 && !strcmp(argv[1], "-c");
    char **prog = argv + (summary ? 2 : 1);
    pid_t pid = fork();
    if (pid == 0) { ptrace(PTRACE_TRACEME, 0, 0, 0); execvp(prog[0], prog); _exit(127); }
    int st, in_call = 0; long count[512] = {0};
    waitpid(pid, &st, 0);
    ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);
    for (;;) {
        ptrace(PTRACE_SYSCALL, pid, 0, 0);
        waitpid(pid, &st, 0);
        if (WIFEXITED(st)) break;
        if (WIFSTOPPED(st) && (WSTOPSIG(st) & 0x80)) {
            struct user_regs_struct r; ptrace(PTRACE_GETREGS, pid, 0, &r);
            if (!in_call) {
                in_call = 1;
                long n = (long)r.orig_rax; if (n >= 0 && n < 512) count[n]++;
                if (!summary) {
                    const char *nm = name(n);
                    if (nm) fprintf(stderr, "%s(%#llx, %#llx, %llu)", nm, r.rdi, r.rsi, r.rdx);
                    else fprintf(stderr, "syscall_%ld(...)", n);
                }
            } else {
                in_call = 0;
                if (!summary) fprintf(stderr, " = %lld\n", (long long)r.rax);
            }
        }
    }
    if (summary) {
        fprintf(stderr, "%-18s %8s\n", "syscall", "calls");
        long tot = 0;
        for (int i = 0; i < 512; i++) if (count[i]) { const char *nm = name(i); char b[32]; if (!nm) { snprintf(b, sizeof b, "syscall_%d", i); nm = b; } fprintf(stderr, "%-18s %8ld\n", nm, count[i]); tot += count[i]; }
        fprintf(stderr, "%-18s %8ld\n", "total", tot);
    }
    return 0;
}
