/* Page-replacement experiment: 100,000 references, 50 pages; strong locality vs random; FIFO, LRU, Clock, Optimal; 3..10 frames. */
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define N 100000
#define PAGES 50
static int ref[N], nextuse[N];
static volatile long sink;
static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec / 1e9; }
static int find(const int *f, int n, int p) { for (int i = 0; i < n; i++) if (f[i] == p) return i; return -1; }
static int fifo(int fr) { int f[16], used = 0, nx = 0, flt = 0; for (int i = 0; i < N; i++) { if (find(f, used, ref[i]) >= 0) continue; flt++; if (used < fr) f[used++] = ref[i]; else { f[nx] = ref[i]; nx = (nx + 1) % fr; } } return flt; }
static int lru(int fr) { int f[16], last[16], used = 0, flt = 0; for (int i = 0; i < N; i++) { int k = find(f, used, ref[i]); if (k >= 0) { last[k] = i; continue; } flt++; if (used < fr) { f[used] = ref[i]; last[used++] = i; } else { int v = 0; for (int j = 1; j < fr; j++) if (last[j] < last[v]) v = j; f[v] = ref[i]; last[v] = i; } } return flt; }
static int clock_(int fr) { int f[16], bit[16], used = 0, hand = 0, flt = 0; for (int i = 0; i < N; i++) { int k = find(f, used, ref[i]); if (k >= 0) { bit[k] = 1; continue; } flt++; if (used < fr) { f[used] = ref[i]; bit[used++] = 1; } else { while (bit[hand]) { bit[hand] = 0; hand = (hand + 1) % fr; } f[hand] = ref[i]; bit[hand] = 1; hand = (hand + 1) % fr; } } return flt; }
static int opt(int fr) { int f[16], nu[16], used = 0, flt = 0; for (int i = 0; i < N; i++) { int k = find(f, used, ref[i]); if (k >= 0) { nu[k] = nextuse[i]; continue; } flt++; if (used < fr) { f[used] = ref[i]; nu[used++] = nextuse[i]; } else { int v = 0; for (int j = 1; j < fr; j++) if (nu[j] > nu[v]) v = j; f[v] = ref[i]; nu[v] = nextuse[i]; } } return flt; }
static void prepare(void) { int last[PAGES]; for (int p = 0; p < PAGES; p++) last[p] = N; for (int i = N - 1; i >= 0; i--) { nextuse[i] = last[ref[i]]; last[ref[i]] = i; } }
static void gen_locality(void) { srand(11); int base = 0; for (int i = 0; i < N; i++) { if (i % 500 == 0) base = rand() % PAGES; ref[i] = (base + rand() % 6) % PAGES; if (rand() % 100 < 4) ref[i] = rand() % PAGES; } }   /* working set of 6 pages that moves; 4% noise */
static void gen_random(void) { srand(12); for (int i = 0; i < N; i++) ref[i] = rand() % PAGES; }
int main(int argc, char **argv)
{
    int csv = argc > 1;
    for (int w = 0; w < 2; w++) {
        if (w == 0) gen_locality(); else gen_random();
        prepare();
        if (!csv) printf("\n%s reference string (%d references over %d pages): page faults\n%-7s %8s %8s %8s %8s\n", w ? "RANDOM" : "STRONG-LOCALITY", N, PAGES, "frames", "FIFO", "LRU", "Clock", "Optimal");
        for (int fr = 3; fr <= 10; fr++) {
            if (csv) printf("%s,%d,%d,%d,%d,%d\n", w ? "random" : "locality", fr, fifo(fr), lru(fr), clock_(fr), opt(fr));
            else printf("%-7d %8d %8d %8d %8d\n", fr, fifo(fr), lru(fr), clock_(fr), opt(fr));
        }
        if (!csv) {
            double t; printf("running time for %d references, 8 frames (milliseconds):", N);
            t = now(); for (int r = 0; r < 20; r++) sink += fifo(8); printf(" FIFO %.2f", (now() - t) * 1e3 / 20);
            t = now(); for (int r = 0; r < 20; r++) sink += lru(8); printf(", LRU %.2f", (now() - t) * 1e3 / 20);
            t = now(); for (int r = 0; r < 20; r++) sink += clock_(8); printf(", Clock %.2f", (now() - t) * 1e3 / 20);
            t = now(); for (int r = 0; r < 20; r++) sink += opt(8); printf(", Optimal %.2f\n", (now() - t) * 1e3 / 20);
        }
    }
    return 0;
}
