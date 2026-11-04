#include <stdio.h>
#include <stdlib.h>
int main() {
    int n, h, i, t = 0, r[50];
    printf("Enter count and head: ");
    scanf("%d %d", &n, &h);
    printf("Enter requests: ");
    for (i = 0; i < n; i++) scanf("%d", &r[i]);
    for (i = 0; i < n; i++) { t += abs(r[i] - h); h = r[i]; }
    printf("Total seek = %d\n", t);
}