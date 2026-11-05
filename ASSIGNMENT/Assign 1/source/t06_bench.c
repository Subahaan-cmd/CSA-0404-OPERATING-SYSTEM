/* Tasks 6 and 10: copy a 4 MB file with read()/write() (several buffer sizes),
   with fread()/fwrite() and with mmap(); report calls and times. */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/resource.h>

#define FILESZ (4L * 1024 * 1024)

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec / 1e9; }
static double tv(struct timeval x, struct timeval y) { return (x.tv_sec - y.tv_sec) + (x.tv_usec - y.tv_usec) / 1e6; }

static long copy_rw(const char *s, const char *d, size_t bs)
{
    char *buf = malloc(bs);
    int in = open(s, O_RDONLY), out = open(d, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    long calls = 0; ssize_t n;
    while ((n = read(in, buf, bs)) > 0) { calls++; if (write(out, buf, n) != n) break; }
    close(in); close(out); free(buf); return calls;
}
static long copy_stdio(const char *s, const char *d, size_t bs)
{
    FILE *in = fopen(s, "r"), *out = fopen(d, "w");
    char *buf = malloc(bs); long calls = 0; size_t n;
    while ((n = fread(buf, 1, bs, in)) > 0) { calls++; fwrite(buf, 1, n, out); }
    fclose(in); fclose(out); free(buf); return calls;
}
static void copy_mmap(const char *s, const char *d)
{
    int in = open(s, O_RDONLY), out = open(d, O_RDWR | O_CREAT | O_TRUNC, 0644);
    struct stat st; fstat(in, &st);
    if (ftruncate(out, st.st_size)) return;
    void *a = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, in, 0);
    void *b = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, out, 0);
    memcpy(b, a, st.st_size);
    munmap(a, st.st_size); munmap(b, st.st_size); close(in); close(out);
}
#define MEASURE(label, calls_expr) do { struct rusage a, b; getrusage(RUSAGE_SELF, &a); double t0 = now(); \
    long c = (calls_expr); double t1 = now(); getrusage(RUSAGE_SELF, &b); \
    printf("%-24s %10ld %9.4f %9.4f %9.4f\n", label, c, t1 - t0, tv(b.ru_utime, a.ru_utime), tv(b.ru_stime, a.ru_stime)); fflush(stdout); } while (0)

int main(void)
{
    char *chunk = malloc(1 << 20); memset(chunk, 'x', 1 << 20);
    int fd = open("src.dat", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    for (long i = 0; i < FILESZ >> 20; i++) if (write(fd, chunk, 1 << 20) < 0) return 1;
    close(fd);
    printf("%-24s %10s %9s %9s %9s\n", "method (4 MB file)", "read calls", "real(s)", "user(s)", "sys(s)");
    size_t sizes[] = {1, 16, 512, 4096, 65536, 1048576};
    for (int i = 0; i < 6; i++) {
        char nm[40]; snprintf(nm, sizeof nm, "read/write b=%zu", sizes[i]);
        MEASURE(nm, copy_rw("src.dat", "dst.dat", sizes[i]));
    }
    size_t fs[] = {16, 4096, 65536};
    for (int i = 0; i < 3; i++) {
        char nm[40]; snprintf(nm, sizeof nm, "fread/fwrite b=%zu", fs[i]);
        MEASURE(nm, copy_stdio("src.dat", "dst.dat", fs[i]));
    }
    MEASURE("mmap + memcpy", (copy_mmap("src.dat", "dst.dat"), 0L));
    unlink("src.dat"); unlink("dst.dat");
    return 0;
}
