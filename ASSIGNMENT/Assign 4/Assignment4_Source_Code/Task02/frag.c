/* frag.c - external fragmentation of contiguous allocation under random create/delete.
   Disk = 2000 blocks. Files of 5-60 blocks are created and deleted at random while the disk
   is kept about 80 percent full. A request "fails because of fragmentation" when enough free
   blocks exist in total (indexed allocation would succeed) but no single hole is big enough. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BLOCKS 2000
#define MAXF 400
static int owner[BLOCKS];                    /* 0 = free, else file id */
static int fstart[MAXF], flen[MAXF], alive[MAXF];

static int find(int size, int policy)        /* 0 = first fit, 1 = best fit, 2 = worst fit */
{
    int best = -1, bestlen = (policy == 1) ? 1 << 30 : -1, run = 0;
    for (int b = 0; b <= BLOCKS; b++) {
        if (b < BLOCKS && owner[b] == 0) { run++; continue; }
        if (run >= size) {
            int st = b - run;
            if (policy == 0) return st;
            if (policy == 1 && run < bestlen) { bestlen = run; best = st; }
            if (policy == 2 && run > bestlen) { bestlen = run; best = st; }
        }
        run = 0;
    }
    return best;
}
static int largest_hole(void)
{
    int run = 0, big = 0;
    for (int b = 0; b < BLOCKS; b++) { if (owner[b] == 0) { run++; if (run > big) big = run; } else run = 0; }
    return big;
}
int main(void)
{
    const char *nm[3] = {"first fit", "best fit", "worst fit"};
    printf("%-10s %9s %12s %14s %12s %14s\n", "policy", "requests", "failed total", "frag. failures", "failure %", "ext.frag % (end)");
    for (int pol = 0; pol < 3; pol++) {
        srand(12345);
        memset(owner, 0, sizeof owner); memset(alive, 0, sizeof alive);
        int req = 0, fail = 0, fragfail = 0, nid = 1, used = 0;
        for (int step = 0; step < 20000; step++) {
            int target = BLOCKS * 80 / 100;
            if (used > target || (rand() % 100 < 45 && used > BLOCKS / 4)) {      /* delete a random live file */
                int k = rand() % MAXF, tries = 0;
                while (!alive[k] && tries++ < MAXF) k = (k + 1) % MAXF;
                if (alive[k]) { for (int i = 0; i < flen[k]; i++) owner[fstart[k] + i] = 0; used -= flen[k]; alive[k] = 0; }
            } else {
                int size = 5 + rand() % 56, slot = -1;
                for (int k = 0; k < MAXF; k++) if (!alive[k]) { slot = k; break; }
                if (slot < 0) continue;
                req++;
                int st = find(size, pol);
                if (st < 0) { fail++; if (BLOCKS - used >= size) fragfail++; continue; }
                for (int i = 0; i < size; i++) owner[st + i] = nid;
                nid++; fstart[slot] = st; flen[slot] = size; alive[slot] = 1; used += size;
            }
        }
        int freeb = BLOCKS - used, big = largest_hole();
        printf("%-10s %9d %12d %14d %11.1f%% %13.1f%%\n", nm[pol], req, fail, fragfail, 100.0 * fail / req,
               freeb ? 100.0 * (freeb - big) / freeb : 0.0);
    }
    return 0;
}
