/* sim4.c - disk-scheduling simulation: 10,000 random requests on cylinders 0-199.
   Service time = 4 ms (rotation + transfer) + 0.1 ms per cylinder of head movement. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAXCYL 199
#define NREQ 10000
enum { FCFS, SSTF, SCAN, CSCAN, LOOK, CLOOK, NALG };
static const char *names[NALG] = {"FCFS", "SSTF", "SCAN", "C-SCAN", "LOOK", "C-LOOK"};
typedef struct { double arr; int cyl; int done; } Req;
static Req r[NREQ];
static unsigned long long rs;

static double urand(void)                     /* xorshift64 generator, fixed seed */
{ rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return (rs >> 11) * (1.0 / 9007199254740992.0); }

static void generate(double mean_gap)         /* Poisson arrivals, uniform cylinders */
{
    double t = 0;
    rs = 88172645463325252ULL;                /* identical workload for every algorithm */
    for (int i = 0; i < NREQ; i++) {
        t += -mean_gap * log(1.0 - urand());
        r[i].arr = t; r[i].cyl = (int)(urand() * (MAXCYL + 1)); r[i].done = 0;
    }
}

/* nearest pending request from 'head' in direction dir (+1 up, -1 down), or -1 */
static int nearest(int first, double now, int head, int dir)
{
    int best = -1, bd = 1 << 30;
    for (int i = first; i < NREQ && r[i].arr <= now; i++) {
        if (r[i].done) continue;
        int d = (r[i].cyl - head) * dir;
        if (d >= 0 && d < bd) { bd = d; best = i; }
    }
    return best;
}

/* choose the next request; *extra receives the extra head travel (run to the end or jump) */
static int pick(int alg, int first, double now, int head, int *dir, int *extra)
{
    int best = -1, bd = 1 << 30;
    *extra = 0;
    switch (alg) {
    case FCFS:
        for (int i = first; i < NREQ && r[i].arr <= now; i++) if (!r[i].done) return i;
        return -1;
    case SSTF:
        for (int i = first; i < NREQ && r[i].arr <= now; i++) {
            if (r[i].done) continue;
            int d = abs(r[i].cyl - head);
            if (d < bd) { bd = d; best = i; }
        }
        return best;
    case SCAN: case LOOK:
        best = nearest(first, now, head, *dir);
        if (best >= 0) return best;
        if (alg == SCAN) {                    /* SCAN runs on to the end of the disk first */
            *extra = (*dir > 0) ? MAXCYL - head : head;
            head = (*dir > 0) ? MAXCYL : 0;
        }
        *dir = -*dir;
        best = nearest(first, now, head, *dir);
        return best;
    case CSCAN: case CLOOK:                   /* always sweep upwards */
        best = nearest(first, now, head, +1);
        if (best >= 0) return best;
        for (int i = first; i < NREQ && r[i].arr <= now; i++) {   /* wrap to the lowest request */
            if (r[i].done) continue;
            if (r[i].cyl < bd) { bd = r[i].cyl; best = i; }
        }
        if (best >= 0)
            *extra = (alg == CSCAN) ? (MAXCYL - head) + MAXCYL    /* to the end, then jump to 0 */
                                    : head - r[best].cyl;         /* C-LOOK: jump straight back */
        return best;
    }
    return -1;
}

static int cmpd(const void *a, const void *b)
{ double x = *(const double *)a, y = *(const double *)b; return (x > y) - (x < y); }

int main(void)
{
    const char *load[3] = {"light", "medium", "heavy"};
    double gap[3] = {40.0, 14.0, 9.0};        /* mean ms between arrivals */
    static double waits[NREQ];
    double bandsum[4], bandn[4];
    printf("%-7s %-7s %9s %10s %10s %10s %7s\n",
           "load", "alg", "avg move", "avg wait", "p99 wait", "max wait", "util%");
    for (int L = 0; L < 3; L++)
        for (int a = 0; a < NALG; a++) {
            generate(gap[L]);
            int head = 100, dir = 1, served = 0, first = 0;
            double t = 0, move = 0, sumw = 0, maxw = 0, busy = 0;
            for (int k = 0; k < 4; k++) bandsum[k] = bandn[k] = 0;
            while (served < NREQ) {
                while (first < NREQ && r[first].done) first++;
                int extra, i = pick(a, first, t, head, &dir, &extra);
                if (i < 0) { t = r[first].arr; continue; }     /* idle until the next arrival */
                /* distance travelled = (extra travel) + (leg from where the head then is) */
                int from = head, d;
                if (a == SCAN && extra)  { from = (dir > 0) ? 0 : MAXCYL; d = extra + abs(r[i].cyl - from); }
                else if (a == CSCAN && extra) d = extra + r[i].cyl;
                else if (a == CLOOK && extra) d = extra;
                else d = abs(r[i].cyl - head);
                double svc = 4.0 + 0.1 * d, w = t - r[i].arr;
                waits[served] = w; sumw += w;
                bandsum[r[i].cyl / 50] += w; bandn[r[i].cyl / 50] += 1; if (w > maxw) maxw = w;
                t += svc; busy += svc; move += d;
                head = r[i].cyl; r[i].done = 1; served++;
            }
            qsort(waits, NREQ, sizeof waits[0], cmpd);
            printf("%-7s %-7s %9.1f %10.1f %10.1f %10.1f %7.1f\n", load[L], names[a],
                   move / NREQ, sumw / NREQ, waits[(int)(0.99 * NREQ)], maxw, 100.0 * busy / t);
            if (L == 2 && a >= SCAN)              /* mean wait by cylinder band, heavy load */
                printf("        mean wait by band 0-49 %.1f | 50-99 %.1f | 100-149 %.1f | 150-199 %.1f ms\n",
                       bandsum[0] / bandn[0], bandsum[1] / bandn[1], bandsum[2] / bandn[2], bandsum[3] / bandn[3]);
        }
    return 0;
}
