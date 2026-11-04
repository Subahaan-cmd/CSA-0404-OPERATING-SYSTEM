#include <stdio.h>

int main()
{
    int disk[50] = {0}, idx, n, blk[10], i;

    printf("Enter index block: ");
    scanf("%d", &idx);

    printf("Enter number of blocks and block numbers: ");
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &blk[i]);
        if (disk[blk[i]] || blk[i] == idx) { printf("Block %d already in use\n", blk[i]); return 0; }
    }

    disk[idx] = 1;
    printf("Index block %d ->", idx);
    for (i = 0; i < n; i++) {
        disk[blk[i]] = 1;
        printf(" %d", blk[i]);                     /* i-th entry points to i-th block */
    }
    printf("\n");
    return 0;
}