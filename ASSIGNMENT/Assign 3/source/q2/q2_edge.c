/* Test addresses at the start and end of a page and beyond the last page (page size 1024, 8 pages). */
#include <stdio.h>
#define PS 1024
#define PAGES 8
int pt[PAGES] = {5, 2, -1, 7, 0, -1, 3, -1};
int main(void)
{
    unsigned t[] = {0, 1023, 1024, 2047, 2048, 7167, 8191, 8192, 8193};
    for (int i = 0; i < 9; i++) {
        unsigned p = t[i] / PS, d = t[i] % PS;
        printf("address %5u: page %u offset %4u -> ", t[i], p, d);
        if (p >= PAGES) puts("INVALID (beyond the last page: trap, process killed)");
        else if (pt[p] < 0) puts("page fault (valid page, not in memory)");
        else printf("physical %u (frame %d)\n", pt[p] * PS + d, pt[p]);
    }
    return 0;
}
