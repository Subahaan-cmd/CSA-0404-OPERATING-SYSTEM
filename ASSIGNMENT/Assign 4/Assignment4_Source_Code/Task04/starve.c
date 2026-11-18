/* starve.c - request at cylinder 183 versus a stream of requests near cylinder 60.
   One new request arrives every 1 ms; each service takes 1 ms (seek + transfer). */
#include <stdio.h>
#include <stdlib.h>
#define MAXQ 100000
int main(void)
{
    for (int alg = 0; alg < 2; alg++) {          /* 0 = SSTF, 1 = SCAN (upwards first) */
        int q[MAXQ], n = 0, head = 53, dir = 1, served = 0;
        srand(1);
        q[n++] = 183;                            /* the victim */
        int t, victim_time = -1;
        for (t = 1; t <= 5000; t++) {
            q[n++] = 55 + rand() % 11;           /* new request near cylinder 60 */
            int best = -1, bd = 1 << 30;
            if (alg == 0) {
                for (int i = 0; i < n; i++) { int d = abs(q[i] - head); if (d < bd) { bd = d; best = i; } }
            } else {
                for (int pass = 0; pass < 2 && best < 0; pass++) {
                    for (int i = 0; i < n; i++) { int d = (q[i] - head) * dir; if (d >= 0 && d < bd) { bd = d; best = i; } }
                    if (best < 0) dir = -dir;
                }
            }
            head = q[best];
            if (q[best] == 183 && victim_time < 0) victim_time = t;
            q[best] = q[--n]; served++;
            if (victim_time >= 0) break;
        }
        if (victim_time >= 0) printf("%s: request at 183 served after %d ms (%d requests served first)\n",
                                      alg ? "SCAN" : "SSTF", victim_time, served - 1);
        else printf("%s: request at 183 STILL WAITING after 5000 ms (%d requests served, queue length %d)\n",
                    alg ? "SCAN" : "SSTF", served, n);
    }
    return 0;
}
