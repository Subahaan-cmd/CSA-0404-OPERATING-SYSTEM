/* Count the page faults of a program: touch N pages of fresh memory and read the minor-fault counter. */
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/resource.h>
static long minflt(void) { struct rusage r; getrusage(RUSAGE_SELF, &r); return r.ru_minflt; }
int main(void)
{
    long ps = sysconf(_SC_PAGESIZE);
    size_t pages = 25600;                         /* 100 MB with 4 KB pages */
    char *m = mmap(NULL, pages * ps, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    long f0 = minflt();
    for (size_t i = 0; i < pages; i++) m[i * ps] = 1;
    long f1 = minflt();
    for (size_t i = 0; i < pages; i++) m[i * ps] = 2;
    long f2 = minflt();
    printf("page size = %ld bytes\nfirst pass over %zu pages : %ld page faults\nsecond pass (pages now mapped): %ld page faults\n", ps, pages, f1 - f0, f2 - f1);
    return 0;
}
