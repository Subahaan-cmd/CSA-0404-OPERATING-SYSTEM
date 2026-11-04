#include <stdio.h>
#include <stdlib.h>
int cmp(const void *a, const void *b) { return *(int *)a - *(int *)b; }
int main() {
    int n, h, size, i, k = 0, t = 0, r[50];
    printf("Enter count, head, disk size: ");
    scanf("%d %d %d", &n, &h, &size);
    printf("Enter requests: ");
    for (i = 0; i < n; i++) scanf("%d", &r[i]);
    qsort(r, n, sizeof(int), cmp);
    while (k < n && r[k] < h) k++;                 // first request >= head
    for (i = k; i < n; i++) { t += abs(r[i] - h); h = r[i]; }       // move up
    t += size - 1 - h; h = size - 1;                                 // go to the end
    for (i = k - 1; i >= 0; i--) { t += abs(r[i] - h); h = r[i]; }  // move down
    printf("Total seek = %d\n", t);
}