/* Run-time loading with dlopen()/dlsym(): the library is loaded only when encryption is needed. */
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
int main(int argc, char **argv)
{
    int need = argc > 1 && !strcmp(argv[1], "encrypt");
    printf("start: encryption %s needed\n", need ? "is" : "is NOT");
    if (!need) return 0;
    void *h = dlopen("./libenc.so", RTLD_NOW);
    if (!h) { fprintf(stderr, "cannot load library: %s\n(falling back: records are sent over TLS only)\n", dlerror()); return 2; }
    void (*enc)(char *, size_t) = (void (*)(char *, size_t))dlsym(h, "encrypt");
    int (*ver)(void) = (int (*)(void))dlsym(h, "enc_version");
    if (!enc || !ver) { fprintf(stderr, "symbol missing: %s\n", dlerror()); return 3; }
    char rec[] = "PATIENT-4711";
    enc(rec, strlen(rec));
    printf("loaded library version %d at run time; first byte now %02x\n", ver(), (unsigned char)rec[0]);
    dlclose(h);
    return 0;
}
