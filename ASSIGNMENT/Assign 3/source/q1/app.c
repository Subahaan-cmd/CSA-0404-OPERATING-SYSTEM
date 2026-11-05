#define _DEFAULT_SOURCE
#include <unistd.h>
/* Branch application: encrypts a record with the library and reports memory use. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
size_t strlen(const char *);
int enc_version(void);
void encrypt(char *buf, size_t n);
static long rss_kb(void) { FILE *f = fopen("/proc/self/status", "r"); char l[128]; long v = 0; while (fgets(l, sizeof l, f)) if (!strncmp(l, "VmRSS:", 6)) { v = atol(l + 6); break; } fclose(f); return v; }
int main(int argc, char **argv)
{
    char rec[] = "PATIENT-4711:HbA1c=6.1";
    encrypt(rec, strlen(rec));
    printf("library version %d, ciphertext:", enc_version());
    for (size_t i = 0; i < 8; i++) printf(" %02x", (unsigned char)rec[i]);
    printf("\nresident memory: %ld KB\n", rss_kb());
    if (argc > 1 && !strcmp(argv[1], "--maps")) {
        FILE *f = fopen("/proc/self/maps", "r"); char l[256];
        while (fgets(l, sizeof l, f)) if (strstr(l, "app") || strstr(l, "libenc") || strstr(l, "libc") || strstr(l, "[stack]") || strstr(l, "[heap]") || strstr(l, "ld-linux")) fputs(l, stdout);
        fclose(f);
    }
    if (argc > 1 && !strcmp(argv[1], "--wait")) { sleep(3); }
    return 0;
}
