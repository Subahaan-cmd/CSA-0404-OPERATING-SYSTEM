#include <stdio.h>

int main()
{
    int next[50], n, blk[10], i;

    for (i = 0; i < 50; i++) next[i] = -1;

    printf("Enter number of blocks and block numbers: ");
    scanf("%d", &n);
    for (i = 0; i < n; i++)
        scanf("%d", &blk[i]);
    for (i = 0; i < n - 1; i++)
        next[blk[i]] = blk[i + 1];                 /* each block points to the next */

    printf("Directory: start = %d, end = %d\n", blk[0], blk[n - 1]);
    printf("Chain: ");
    for (i = blk[0]; i != -1; i = next[i])
        printf("%d -> ", i);
    printf("NULL\n");
    return 0;
}